import os

from cs50 import SQL
from datetime import datetime
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd,  validate_username

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
    id = session.get("user_id")
    rows_stocks = db.execute("SELECT symbol, shares FROM quotes WHERE user_id = ?", id)
    rows_cash = db.execute("SELECT cash FROM users WHERE id = ?", id)
    current_cash = 0

    if(len(rows_stocks) == 0):
        rows_stocks = []

    gran_total = 0
    for stock in rows_stocks:
        quote = lookup(stock["symbol"])
        stock["price"] = quote["price"]
        stock["total"] = stock["price"] * stock["shares"]
        gran_total += stock["total"]

    if(len(rows_cash) == 1):
        current_cash = rows_cash[0]["cash"]


    return render_template("index.html", stocks=rows_stocks, gran_total=gran_total + current_cash, current_cash=current_cash)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "GET":
        return render_template("buy.html")

    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        if not shares.isnumeric():
            return apology("Input is not a integer", 400)

        shares = int(shares)
        if shares <= 0:
            return apology("Input must be a positive integer")

        quote = lookup(symbol)
        if not quote:
            return apology("Invalid symbol", 400)

        #Get user id from session
        user_id = session.get("user_id")

        #Get user available cash
        cash_rows = db.execute("SELECT cash FROM users WHERE id = ?", user_id)

        if(len(cash_rows) != 1):
            return apology("Server error", 500)

        user_cash = cash_rows[0].get("cash")
        quote_price = quote.get("price")
        #Calculate necessary amount and validate
        amount = quote_price * shares
        if(user_cash - amount < 0):
            flash("Insuficient money", "error")
            return redirect("/")


        try:
            db.execute("BEGIN")
            # Look if the quotes alredy exist
            rows_quotes = db.execute("SELECT id, shares FROM quotes WHERE user_id = ? AND symbol = ?", user_id, symbol)
            if(len(rows_quotes) == 1):
                #Actualize the value of shares if exist
                db.execute("UPDATE quotes SET shares = ? WHERE id = ?", rows_quotes[0]["shares"] + shares, rows_quotes[0]["id"])
            else:
                #Insert a new quote symbol if not
                db.execute("INSERT INTO quotes (user_id, symbol, shares) VALUES (?, ?, ?)",
                                        user_id, symbol, shares)

            db.execute("INSERT INTO transactions (user_id, symbol, price, shares, total, type, date) VALUES (?, ?, ?, ?, ?, ?, ?)",
                       user_id, symbol, quote_price, shares, amount, 2, datetime.today().isoformat(" ", "minutes"))

            db.execute("UPDATE users SET cash = ? WHERE id = ?", user_cash - amount, user_id)
            db.execute("COMMIT")
            flash("Bought!", "message")
        except:
            db.execute("ROLLBACK")
            return apology("Server error", 500)

    return redirect("/")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    id = session.get("user_id")

    stocks = db.execute("" \
    "SELECT symbol, price, shares, total, tt.desc as type, date " \
    "FROM transactions " \
    "JOIN transactions_type tt ON type = tt.id " \
    "WHERE user_id = ?" \
    "", id)

    for stock in stocks:
        print(stock["date"])
        fdatetime = stock["date"].split()
        stock["time"] = fdatetime[1]
        stock["date"] = fdatetime[0].replace("-", "/")

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
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
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
    if request.method == "GET":
        return render_template("quote.html")

    if request.method == "POST":
        quote = lookup(request.form.get("symbol"))

        if not quote:
            return apology("Inexistent symbol", 400)

        quote["price"] = quote.get("price")

        return render_template("quoted.html", quote=quote)


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    if request.method == "GET":
        return render_template("register.html")
    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        if not username:
            return apology("Invalid username", 400)
        if not password:
            return apology("Invalid password", 400)
        if password != confirmation:
            return apology("passwords must be equals", 400)


        hash = generate_password_hash(password)
        try:
            session["user_id"] = db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, hash)
        except:
            return apology("username alredy in use", 400)

    return redirect("/")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "GET":
        rows = db.execute("SELECT symbol FROM quotes WHERE user_id = ? GROUP BY symbol", session.get("user_id"))
        return render_template("sell.html", stocks=rows)

    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        if not symbol:
            return apology("Must select a symbol", 400)

        if not shares.isnumeric():
            return apology("Input is not a integer", 400)

        shares = int(shares)
        if shares <= 0:
            return apology("Input must be a positive integer")

        #Get user id from session
        user_id = session.get("user_id")

        max_shares = db.execute("" \
        "SELECT sum(shares) as max"
        "   FROM quotes"
        "       WHERE user_id = ?"
        "       AND symbol = ?", user_id, symbol)

        if(shares > max_shares[0]["max"]):
            return apology("Insuficiente shares", 400)

        quote = lookup(symbol)
        if not quote:
            return apology("Invalid symbol", 400)

        #Get user available cash
        cash_rows = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
        if(len(cash_rows) != 1):
            return apology("Server error", 500)

        user_cash = cash_rows[0].get("cash")

        quote_price = quote.get("price")
        amount = quote_price * shares

        rows = db.execute("" \
            "SELECT id, shares " \
            "   FROM quotes" \
            "       WHERE user_id = ?" \
            "       AND symbol = ?", user_id, symbol)

        if(len(rows) != 1):
            return apology("Stock not disponible", 400)

        try:
            db.execute("BEGIN")
            stock_shares = rows[0]["shares"] - shares
            stock_id = rows[0]["id"]

            if stock_shares == 0:
                db.execute("DELETE FROM quotes WHERE id = ?", stock_id)
            else:
                db.execute("UPDATE quotes SET shares = ? WHERE id = ?", stock_shares, stock_id)

            db.execute("INSERT INTO transactions (user_id, symbol, price, shares, total, type, date) VALUES (?, ?, ?, ?, ?, ?, ?)",
                    user_id, symbol, quote_price, shares, amount, 1, datetime.today().isoformat(" ", "minutes"))

            db.execute("UPDATE users SET cash = ? WHERE id = ?", user_cash + amount, user_id)
            db.execute("COMMIT")
            flash("Sold!", "message")
        except:
            db.execute("ROLLBACK")
            return apology("Error selling stocks", 400)

        return redirect("/")




@app.route("/user", methods=["POST"])
@login_required
def user():
    """Returns username"""

    id = session.get("user_id")

    rows = db.execute("SELECT username FROM users WHERE id = ?", id)

    if(len(rows) != 1):
        return ""

    return rows[0].get("username")


@app.route("/user/name", methods=["PATCH"])
@login_required
def user_name():
    if request.method == "PATCH":
        new_username = request.get_json()["username"]

        new_username_query = "%" + new_username + "%"
        rows = db.execute("SELECT count(id) AS counter FROM users WHERE username = ?", new_username_query)

        if(rows[0].get("counter") > 0):
            return "Username alredy in use", 400

        if not validate_username(new_username):
            return "Invalid username", 400

        db.execute("UPDATE users SET username = ? WHERE id = ?", new_username, session.get("user_id"))

        return "Username altered", 200


@app.route("/user/passwd", methods=["PATCH"])
@login_required
def user_passwd():
    if request.method == "PATCH":
        old_passwd = request.form.get("old_passwd")
        new_passwd = request.form.get("new_passwd")
        new_confirmation = request.form.get("new_confirmation")


        rows = db.execute("SELECT hash FROM users WHERE id = ?", session.get("user_id"))

        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], old_passwd
        ):
            return "Server error", 500


        if new_passwd != new_confirmation:
            return "Passwords must be equal", 400

        hash = generate_password_hash(new_passwd)

        try:
            db.execute("UPDATE users SET hash = ? WHERE id = ?", hash, session.get("user_id"))
            return "Password altered", 200
        except:
            return "Server error", 500

