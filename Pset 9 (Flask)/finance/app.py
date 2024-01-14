import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import generate_password_hash, check_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    # first create a LIST OF DICTs to store results from querying the transactions db
    # where we pool all shares of the same symbol together (GROUP BY) and order alphabetically by symbol
    # nb the total_shares alias is now accessible in this dict
    portfolio1 = db.execute(
        "SELECT symbol, SUM(number_of_shares) AS total_shares FROM transactions WHERE user_id = ? GROUP BY symbol HAVING SUM(number_of_shares) > 0 ORDER BY symbol",
        session["user_id"],
    )

    # now create a new LIST OF DICTs to add information about current prices of the relevant stocks
    portfolio2 = []

    # loop through each dict in portfolio1, and populate portfolio2 item by item
    # for each item (company) in portfolio1, we "copy" into portfolio2 the symbol and total number of shares,
    # add the price we've just found, and calculate total cost
    for item1 in portfolio1:
        current_price = lookup(item1["symbol"])["price"]
        if current_price:
            item2 = {
                "symbol": item1["symbol"],
                "shares": item1["total_shares"],
                "price": current_price,
                "total": item1["total_shares"] * current_price,
            }
            portfolio2.append(item2)

    # find how much cash the current user has
    rows = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])
    if not rows:
        return apology("user not found", 403)
    # since the query is expected to return a list of 1 dict, we access the cash info like so:
    cash = rows[0]["cash"]

    # find the total worth of the current user
    total = cash
    # loop through stock holdings to add each and every of them to cash holding
    for item2 in portfolio2:
        total += item2["total"]

    # render HTML page, plugging in the relevant variables
    return render_template("index.html", portfolio=portfolio2, cash=cash, total=total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        # Ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("must specify symbol", 400)

        # Ensure symbol is valid
        elif lookup(request.form.get("symbol")) == None:
            return apology("symbol not found", 400)

        # Ensure shares was submitted
        elif not request.form.get("shares"):
            return apology("must specify shares", 400)

        # Convert shares to integer
        try:
            shares = int(request.form.get("shares"))
        except ValueError:
            return apology("number of shares must be a positive integer", 400)

        # Ensure shares is a POSITIVE integer
        if shares <= 0:
            return apology("number of shares must be a positive integer", 400)

        # Find current price per share
        results = lookup(request.form.get("symbol"))
        # Calculate total cost of purchase
        cost = results["price"] * shares

        # Ensure user has enough cash to buy
        # Access user's cash
        rows = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])
        if not rows:
            return apology("user not found", 400)
        cash = rows[0]["cash"]
        # Compare user's cash to cost of purchase
        if cash < cost:
            return apology("cannot afford :(", 400)

        # Store transaction in transactions table
        db.execute(
            "INSERT INTO transactions (user_id, symbol, number_of_shares, price_per_share) VALUES (?, ?, ?, ?)",
            session["user_id"],
            request.form.get("symbol"),
            shares,
            results["price"],
        )

        # Update current user's cash in users table
        db.execute(
            "UPDATE users SET cash = cash - ? WHERE id = ?", cost, session["user_id"]
        )

        flash("Bought!")
        return redirect("/")

    # if accessed via GET
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    # similar to index, create a LIST OF DICTs to store results from querying the transactions db
    # relevant information on transactions of current user ordered by time
    history = db.execute(
        "SELECT symbol, number_of_shares, price_per_share, timestamp FROM transactions WHERE user_id = ? ORDER BY timestamp",
        session["user_id"],
    )
    # render HTML page with information in the history dict
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
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 400)

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
    if request.method == "POST":
        # Call lookup to find the symbol
        if lookup(request.form.get("symbol")) == None:
            return apology("symbol not found", 400)
        results = lookup(request.form.get("symbol"))
        # render HTML page, passing in values we just found
        return render_template(
            "quoted.html", stock_name=results["name"], value=results["price"]
        )
    # if accessed via GET
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure username does not already exist
        elif (
            len(
                db.execute(
                    "SELECT * FROM users WHERE username = ?",
                    request.form.get("username"),
                )
            )
            != 0
        ):
            return apology("username already taken", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Ensure password was re-entered
        elif not request.form.get("confirmation"):
            return apology("must provide password", 400)

        # Ensure re-entered password matches first password
        elif request.form.get("confirmation") != request.form.get("password"):
            return apology("password must match", 400)

        # Hash password
        hashed = generate_password_hash(
            request.form.get("password"), method="scrypt", salt_length=16
        )
        # Insert new user into users database
        db.execute(
            "INSERT INTO users (username, hash) VALUES (?, ?)",
            request.form.get("username"),
            hashed,
        )

        # Automatically log new user in
        # Retrieve user
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as in by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        # Ensure symbol was selected
        if not request.form.get("symbol"):
            return apology("must specify symbol", 400)

        # Ensure shares was submitted
        elif not request.form.get("shares"):
            return apology("must specify shares", 400)

        # Convert shares to integer
        try:
            shares = int(request.form.get("shares"))
        except ValueError:
            return apology("number of shares must be a positive integer", 400)

        # Ensure shares is a positive integer
        if shares <= 0:
            return apology("number of shares must be a positive integer", 400)

        # Ensure user has enough shares to sell
        rows = db.execute(
            "SELECT SUM(number_of_shares) AS total FROM transactions WHERE user_id = ? AND symbol = ?",
            session["user_id"],
            request.form.get("symbol"),
        )
        if not rows:
            return apology("shares not found", 400)
        total_shares = int(rows[0]["total"])
        if shares > total_shares:
            return apology("not enough shares", 400)

        # Calculate total earning from selling shares
        results = lookup(request.form.get("symbol"))
        earning = results["price"] * shares

        # Store transaction in db, nb number of shares is stored as negative in the case of selling, as opposed to positive in the case of buying
        db.execute(
            "INSERT INTO transactions (user_id, symbol, number_of_shares, price_per_share) VALUES (?, ?, ?, ?)",
            session["user_id"],
            request.form.get("symbol"),
            -shares,
            results["price"],
        )

        # Update user's cash, where earning is added
        db.execute(
            "UPDATE users SET cash = cash + ? WHERE id = ?", earning, session["user_id"]
        )

        flash("Sold!")
        return redirect("/")

    else:
        # Query the transactions db to find companies from whom the user currently holds shares
        # Store non-redundant (DISTINCT) symbols in a list
        options = db.execute(
            "SELECT DISTINCT symbol FROM transactions WHERE user_id = ? ORDER BY symbol",
            session["user_id"],
        )
        # render HTML page, passing options list in to be used as dropdown menu
        return render_template("sell.html", options=options)


"""PERSONAL TOUCH: change password"""


@app.route("/password", methods=["GET", "POST"])
@login_required
def password():
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure old password was submitted
        elif not request.form.get("old_password"):
            return apology("must provide old password", 400)

        # Ensure new password was submitted
        elif not request.form.get("new_password"):
            return apology("must provide new password", 400)

        # Find information of current user in preparation for verification
        user_info = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )
        if not user_info:
            return apology("incorrect username", 400)

        # Ensure the proclaimed username is in session
        if not user_info[0]["id"] == session["user_id"]:
            return apology("incorrect username", 400)

        # Ensure old password is correct
        elif not check_password_hash(
            user_info[0]["hash"], request.form.get("old_password")
        ):
            return apology("incorrect password", 400)

        # Ensure new password is different
        elif request.form.get("new_password") == request.form.get("old_password"):
            return apology("new password must be different", 400)

        # Hash new password
        hashed = generate_password_hash(
            request.form.get("new_password"), method="scrypt", salt_length=16
        )

        # Update password in users db
        db.execute("UPDATE users SET hash = ? WHERE id = ?", hashed, session["user_id"])

        # Redirect user to home page
        flash("Password change successful")
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("password.html")
