import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime

from helpers import apology, login_required, lookup, usd

# Ensure environment variable is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # Get which stocks the user owns, the numbers of shares owned
    rows = db.execute("SELECT stocks.name, SUM(number) as sum FROM transactions INNER JOIN stocks "
                      "ON transactions.stockid = stocks.id WHERE userid = :userid "
                      "GROUP BY userid, stockid HAVING sum > 0 ORDER BY date DESC", userid=session["user_id"])

    # Get user's cash
    cash = db.execute("SELECT cash FROM users WHERE id = :userid", userid=session["user_id"])

    cash = cash[0]["cash"]
    total = cash
    # Price for each symbol
    prices = []

    # get symbol's price & calculate total
    for i in range(len(rows)):
        stock = lookup(rows[i]["name"])
        prices.append(stock["price"])
        total += (prices[i] * rows[i]["sum"])

    return render_template("index.html", rows=rows, prices=prices, cash=cash, total=total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # User reached route via POST
    if request.method == "POST":

        # Ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("Missing symbol")

        # Ensure shares was submitted
        if not request.form.get("shares"):
            return apology("Missing shares")

        # Handles fractional, negative, and non-numeric shares
        try:
            if int(request.form.get("shares")) < 0:
                return apology("Invalid shares")
        except:
            return apology("Invalid shares")

        # Get stock's current price
        stock = lookup(request.form.get("symbol"))

        # if symbol doesn't exist
        if not stock:
            return apology("Invalid symbol")

        # Get user's cash from db
        result = db.execute("SELECT cash FROM users WHERE id = :id",
                            id=session["user_id"])

        cost = stock["price"] * int(request.form.get("shares"))
        # if the user cannot afford
        if cost > result[0]["cash"]:
            return apology("Can't afford")

        result = db.execute("SELECT * FROM stocks WHERE name = :symbol",
                            symbol=stock["symbol"])

        # Check if stock was exist in db table 'stocks', if not insert stock to stocks table
        if not result:
            stockid = db.execute("INSERT INTO stocks (name) VALUES (:symbol)", symbol=stock["symbol"])
        else:
            stockid = result[0]["id"]

        # Get current date
        date = datetime.now().strftime("%Y-%m-%d %H:%M:%S")

        db.execute("INSERT INTO transactions (userid, stockid, number, price, date) "
                   "VALUES (:userid, :stockid, :number, :price, :date)",
                   userid=session["user_id"], stockid=stockid, number=int(request.form.get("shares")),
                   price=stock["price"], date=date)

        # Update user's cash
        db.execute("UPDATE users SET cash = cash - :cost WHERE id = :id", cost=cost, id=session["user_id"])

        flash("Bought!")
        # Redirect user to home page
        return redirect("/")

     # User reached route via GET
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    history = db.execute("SELECT name, number, price, date FROM transactions INNER JOIN stocks "
                         "ON transactions.stockid = stocks.id WHERE userid = :userid", userid=session["user_id"])

    return render_template("history.html", history=history)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    # User reached route via POST
    if request.method == "POST":

        # Ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("Missing symbol")

        # Get symbol's detail
        quote = lookup(request.form.get("symbol"))

        # If symbol is invalid
        if not quote:
            return apology("Invalid symbol")

        return render_template("quoted.html", quote=quote)

    # User reached route via GET
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("Missing username!")

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("Missing password")

        # Ensure password and password confirmation are equal
        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("Passwords don't match")

        hashedPass = generate_password_hash(request.form.get("password"))

        result = db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)",
                            username=request.form.get("username"), hash=hashedPass)

        # Ensure there isn't another user with this username
        if not result:
            return apology("Username taken")

        # Remember which user has logged in
        session["user_id"] = result

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    symbols = db.execute("SELECT stocks.name, SUM(number) as sum FROM transactions INNER JOIN stocks "
                         "ON transactions.stockid = stocks.id WHERE userid = :userid "
                         "GROUP BY stockid HAVING sum > 0", userid=session["user_id"])

    if request.method == "POST":

        for i in range(len(symbols)):
            if symbols[i]["name"] == request.form.get("symbol"):
                if symbols[i]["sum"] < int(request.form.get("shares")):
                    return apology("Too many shares")

        stockid = db.execute("SELECT id FROM stocks where name = :name", name=request.form.get("symbol"))
        stock = lookup(request.form.get("symbol"))

        # Get current date
        date = datetime.now().strftime("%Y-%m-%d %H:%M:%S")

        db.execute("INSERT INTO transactions (userid, stockid, number, price, date) "
                   "VALUES (:userid, :stockid, :number, :price, :date)",
                   userid=session["user_id"], stockid=stockid[0]["id"], number=-1 * int(request.form.get("shares")),
                   price=stock["price"], date=date)

        # Update user's cash
        db.execute("UPDATE users SET cash = cash + :cost WHERE id = :id",
                   cost=stock["price"] * int(request.form.get("shares")), id=session["user_id"])

        flash("Sold!")
        return redirect("/")

    else:
        return render_template("sell.html", symbols=symbols)


@app.route("/change_password", methods=["GET", "POST"])
@login_required
def change_password():

    # User reached route via POST
    if request.method == "POST":

        # Ensure old password was submitted
        if not request.form.get("oldpass"):
            return apology("Missing Old password!")

        # Ensure new password was submitted
        if not request.form.get("newpass"):
            return apology("Missing New password!")

        # Ensure new password and new password confirmation are equal
        elif request.form.get("newpass") != request.form.get("confirmation"):
            return apology("New Passwords don't match")

        oldpass = db.execute("SELECT hash FROM users WHERE id=:id", id=session["user_id"])

        # Ensure old password is valid
        if not check_password_hash(oldpass[0]["hash"], request.form.get("oldpass")):
            return apology("Invalid Old password")

        # Update users password
        hash = generate_password_hash(request.form.get("newpass"))
        db.execute("UPDATE users SET hash=:hash WHERE id = :id", hash=hash, id=session["user_id"])

        flash("Password has changed!")
        return redirect("/")

    # User reached route via POST
    else:
        return render_template("change_password.html")


def errorhandler(e):
    """Handle error"""
    return apology(e.name, e.code)


# listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
