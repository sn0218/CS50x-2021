import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

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

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/edit", methods=["GET", "POST"])
@login_required
def edit():
    """Change password"""

    # User reached route via POST
    if request.method == "POST":

        # store the input data in variable
        oldpw = request.form.get("oldpassword")
        newpw = request.form.get("newpassword")
        confirm = request.form.get("confirmation")

        # query database
        rows = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])

        # Ensure old password is submitted
        if not oldpw:
            return apology("Missing Old Password", 400)

        # Ensure new password is submitted
        elif not newpw:
            return apology("Missing New Password", 400)

        # Ensure confimration is submitted
        elif not confirm:
            return apology("Missing New Password", 400)

        # Ensure confirmation is matched
        elif not newpw == confirm:
            return apology("New Passwords don't match", 400)

        # Ensure existing password is correct
        elif not check_password_hash(rows[0]["hash"], oldpw):
            return apology("invalid password", 403)

        # hash the new password
        hashpw = generate_password_hash(newpw, method='pbkdf2:sha256', salt_length=8)

        # insert the new user into users table
        db.execute("UPDATE users SET hash = ? WHERE id = ?", hashpw, session["user_id"])

        flash("Password is changed!")

        # redirect the user to the homepage
        return redirect("/")

    # User reached route via GET
    else:
        return render_template("edit.html")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # query for users by session ID
    users = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])
    curCash = float(users[0]["cash"])
    grand_total = curCash

    # query for purchase by session ID
    # stocks return from DB is a dictionary
    stocks = db.execute("SELECT symbol, stockname, SUM(shares) AS shares from purchase WHERE user_id = ? GROUP BY symbol ORDER BY symbol", session["user_id"])

    # iterate over the profolio of the user
    for stock in stocks:
        quote = lookup(stock["symbol"])
        # add new key-value pair in dict of stock
        stock["price"] = quote["price"]
        stock["total"] = stock["price"] * stock["shares"]
        grand_total += stock["total"]

    return render_template("profolio.html", stocks=stocks, grand_total=usd(grand_total), curCash=usd(curCash))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # User reached route via POST
    if request.method == "POST":

        symbol = request.form.get("symbol")

        # handle neagtive and fractional shares
        try:
            shares = int(request.form.get("shares"))
        except:
            return apology("shares must be a positive integer", 400)

        if shares < 1:
            return apology("Enter valid amount of shares", 400)

        # store the return value from lookup()
        quote = lookup(symbol)

        # ensure symbol is submitted
        if not symbol:
            return apology("missing symbol", 400)

        # ensure shares is submitted
        if not shares:
            return apology("missing shares", 400)

        # check the validity of symbol
        if quote == None:
            return apology("Invalid symbol", 400)

        # Query database for cash by session ID
        rows = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])
        cash = float(rows[0]["cash"])
        total = quote["price"] * int(shares)

        # check the user if he/she can afford the current stock
        if total > cash:
            return apology("can't afford", 400)

        # the user can afford the stock and the stock actually exists
        else:

            userid = session["user_id"]
            stockSymbol = quote["symbol"]
            stockName = quote["name"]
            curPrice = float(quote["price"])

            # insert the purchase record into database
            db.execute("INSERT INTO purchase (user_id, symbol, stockname, shares, price) VALUES (?, ?, ?, ?, ?)", userid, stockSymbol, stockName, shares, curPrice)

            # update the cash of the user
            curCash = cash - (curPrice * shares)
            db.execute("UPDATE users SET cash = ? WHERE id = ?", curCash, session["user_id"])

            flash("Bought!")

            return redirect("/")

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # query for purchase by session ID
    # stocks return from DB is a dictionary
    stocks = db.execute("SELECT symbol, shares, price, Timestamp AS transacted from purchase WHERE user_id = ?", session["user_id"])

    return render_template("history.html", stocks=stocks)


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
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

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

    flash("Logout successful!")

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    # User reached route via POST
    if request.method == "POST":
        symbol = request.form.get("symbol")

    # handle different case of input stock's symbol
        if not symbol:
            return apology("Missing symbol", 400)
        elif lookup(symbol) == None:
            return apology("Invalid symbol", 400)
        else:
            # shows stores the dictionary return by lookup() function
            # on the left is the name used in quoted.html
            # on the right is the python variable
            shows = lookup(symbol)
            return render_template("quoted.html", shows=shows)

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # Forget any user_id
    session.clear()

    # User reached route via POST
    if request.method == "POST":

        # store the input data in variable
        username = request.form.get("username")
        pw1 = request.form.get("password")
        pw2 = request.form.get("confirmation")

        # Ensure username is submitted
        if not username:
            return apology("Missing username", 400)

        # Ensure password is submitted
        elif not pw1:
            return apology("Missing password", 400)

        # Ensure confimration is submitted
        elif not pw2:
            return apology("Missing password", 400)

        # Ensure confirmation is matched
        elif not pw1 == pw2:
            return apology("Passwords don't match", 400)

        # query database for username to check the availability
        rows = db.execute("SELECT * FROM users WHERE username = ?", username)

        # check the username's availability
        if len(rows) == 1:
            return apology("The username is not available", 400)

        # hash the password
        hashpw = generate_password_hash(request.form.get("password"), method='pbkdf2:sha256', salt_length=8)

        # insert the new user into users table
        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, hashpw)

        # Query database again for username for login
        rows = db.execute("SELECT * FROM users WHERE username = ?", username)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        flash("Registered!")

        # redirect the user to the homepage
        return redirect("/")

    # User reached route via GET
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # User reached route via POST
    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        # ensure symbol is submitted
        if not symbol:
            return apology("missing symbol", 400)

        # ensure shares is submitted
        if not shares:
            return apology("missing shares", 400)

        # store the return value from lookup()
        quote = lookup(symbol)

        shares = int(shares)

        # Query database for shares by session ID and selected stock
        rows = db.execute("SELECT SUM(shares) AS shares FROM purchase WHERE user_id = ? AND symbol = ?", session["user_id"], symbol)
        Actualshares = rows[0]["shares"]

        # check the user if he/she can sell the shares of stock
        if shares > Actualshares:
            return apology("too many shares", 400)

        # the user can sell the specified number of shares of the stock
        else:

            # Query database for cash by session ID
            rows = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])
            cash = float(rows[0]["cash"])

            # update the cash of the user
            curPrice = float(quote["price"])
            curCash = cash + (curPrice * shares)
            db.execute("UPDATE users SET cash = ? WHERE id = ?", curCash, session["user_id"])

            # update the purchase table to indicate some number of shares are sold
            userid = session["user_id"]
            stockSymbol = quote["symbol"]
            stockName = quote["name"]

            # convert number of share sold to negative value
            shares = -1 * shares

            # insert the purchase record into database of selling the stock
            # neagative value of shares mean that the shares of stock is sold
            db.execute("INSERT INTO purchase (user_id, symbol, stockname, shares, price) VALUES (?, ?, ?, ?, ?)", userid, stockSymbol, stockName, shares, curPrice)

            flash("Sold!")

            return redirect("/")

    # User reached route via GET
    else:
        stocks = db.execute("SELECT DISTINCT symbol FROM purchase WHERE user_id = ? ORDER BY symbol", session["user_id"])
        return render_template("sell.html", stocks=stocks)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
