import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime

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

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


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
    # get user info
    user_id = session["user_id"]
    username = db.execute("SELECT username FROM users WHERE id = ?", user_id)[0]["username"]
    cash = usd(db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"])

    # grab the shares owned by the user
    stocks_owned = db.execute("SELECT symbol, shares FROM holdings WHERE user_id = ?", user_id)

    # variable to track total
    sum_tot = 0

    # lookup price of users stocks and add them to the stocks_owned list of dictionaries
    for stock in stocks_owned:
        lookup_stocks = lookup(stock["symbol"])
        stock["price"] = lookup_stocks["price"]
        stock["name"] = lookup_stocks["name"]
        stock["total"] = round((float(stock["price"]) * float(stock["shares"])), 2)
        sum_tot += stock["total"]

    # Stocks_owned now contains symbol, shares, price, name
    # Iterate over stocks_owned to convert to usd

    for stock in stocks_owned:
        stock["price"] = usd(stock["price"])

    # Convert total to usd
    sum_tot = usd(sum_tot)

    return render_template("index.html", username=username, stocks_owned=stocks_owned, sum_tot=sum_tot, cash=cash)


@app.route("/deposit", methods=["GET", "POST"])
@login_required
def deposit():
    if request.method == "GET":
        return render_template("deposit.html")
    else:
        # get user info
        user_id = session["user_id"]
        cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]

        # update user table with deposit
        deposit = int(request.form.get("deposit"))
        updated_cash = cash + deposit
        if deposit > 0:
            db.execute("UPDATE users SET cash = ? WHERE id = ?", updated_cash, user_id)
            flash('Deposit Successful')
            return redirect("/")
        else:
            return apology("Deposit Unsuccessful", 400)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "GET":
        return render_template("buy.html")
    else:
        # define stock sybol and number of shares for user input in form
        symbol = request.form.get("symbol")
        n_shares = request.form.get("shares")
        stock_info = lookup(symbol)

        if not n_shares:
            return apology("Please provide a number of shares to purchase", 400)
        elif not symbol or not stock_info:
            return apology("Please provide a valid symbol", 400)

        # convert n_shares from str to float
        if n_shares.isnumeric():
            n_shares = float(n_shares)
        else:
            return apology("Please provide a positive integer number of shares", 400)

        if n_shares <= 0 or n_shares % 1 != 0:
            return apology("Please provide a positive integer number of shares", 400)
        else:
            price = stock_info["price"]
            user_id = session["user_id"]

            # cost of purchase
            cost = n_shares * price

            # check the user can afford the purchase, return an error if not
            cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
            user_cash = cash[0]["cash"]
            if cost > user_cash:
                return apology("insufficient funds remaining", 400)
            else:
                cash_remaining = user_cash - cost

                # update user table with the cost taken off the user's cash
                db.execute("UPDATE users SET cash = ? WHERE id = ?", cash_remaining, user_id)

                # update order_history table for this order
                db.execute("INSERT INTO order_history (user_id, symbol, shares, price, timestamp, bought_sold) VALUES (?, ?, ?, ?, ?, ?)",
                           user_id, symbol, n_shares, price, datetime.now(), "bought")

                # update holdings table to keep track of users shares

                # check if user already has shares of this type
                has_shares = db.execute("SELECT shares FROM holdings WHERE user_id = ? AND symbol =  ?", user_id, symbol)

                # if user doesn't have shares of this type already then just add purchase to the holdings table
                if not has_shares:
                    db.execute("INSERT INTO holdings (user_id, symbol, shares) VALUES (?, ?, ?)", user_id, symbol, n_shares)

                # if user already has some shares then add the number of bought shares to what they already own
                else:
                    shares_tot = has_shares[0]["shares"] + n_shares
                    db.execute("UPDATE holdings SET shares = ? WHERE user_id = ? AND symbol = ?", shares_tot, user_id, symbol)

                flash('Bought!')
                return redirect("/")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    # get user id
    user_id = session["user_id"]
    # get history data
    history = db.execute("SELECT symbol, shares, price, timestamp, bought_sold FROM order_history WHERE user_id = ?", user_id)
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
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
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
    if request.method == "GET":
        return render_template("quote.html")
    else:
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("please enter a stock symbol", 400)
        else:
            stock_info = lookup(symbol)
            if not stock_info:
                return apology("please enter a valid stock symbol", 400)
            else:
                name = stock_info["name"]
                price = usd(stock_info["price"])
                return render_template("quoted.html", name=name, symbol=symbol.upper(), price=price)


@app.route("/register", methods=["GET", "POST"])
def register():
    if request.method == "GET":
        return render_template("register.html")
    else:
        # assign username to a variable
        username = request.form.get("username")

        # check table of registrants to see if username already exists, user_check will be populated by a username is it does already exist
        user_check = db.execute("SELECT username FROM users WHERE username = ?", username)

        # return error message if user doesn't provide a username
        if not username:
            return apology("please enter a username", 400)
        # return error if username already exists
        elif len(user_check) > 0:
            return apology("username already exists", 400)

        # assign password and confirmation to variables and then check password and re-typed password match
        password = request.form.get("password")
        p_confirmation = request.form.get("confirmation")

        if not password:
            return apology("please enter a password")

        elif password != p_confirmation:
            return apology("passwords don't match", 400)

        # hash password
        hash_pass = generate_password_hash(password, method='pbkdf2:sha256', salt_length=8)

        # store user in the database
        db.execute("INSERT INTO users(username, hash) VALUES (?, ?)", username, hash_pass)

        flash('Registered!')
        return render_template("login.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    # grab user data
    user_id = session["user_id"]
    user_stocks = db.execute("SELECT * FROM holdings WHERE user_id = ?", user_id)

    if request.method == "GET":
        # get users stocks to add to the select menu
        return render_template("sell.html", user_stocks=user_stocks)
    else:
        # define stock sybol and number of shares for user input in form
        symbol = request.form.get("symbol")
        sell_shares = int(request.form.get("shares"))

        if not symbol:
            return apology("Please provide a stock symbol", 400)
        elif sell_shares <= 0:
            return apology("Please provide a positive number of shares", 400)
        else:
            # lookup share info
            stock_info = lookup(symbol)

            # check user has the shares and enough of them
            user_stock = db.execute("SELECT symbol, shares FROM holdings WHERE user_id = ? AND symbol = ?", user_id, symbol)

            if not stock_info:
                return apology("please enter a valid stock symbol", 400)
            elif not user_stock[0]["symbol"]:
                return apology("you do not own any of this stock", 400)
            elif user_stock[0]["shares"] < sell_shares:
                return apology("you do not own enough shares", 400)
            else:
                name = stock_info["name"]
                price = stock_info["price"]

                # update holdings table
                new_shares_tot = user_stock[0]["shares"] - sell_shares

                # remove shares from holdings table
                if new_shares_tot > 0:
                    db.execute("UPDATE holdings SET shares = ? WHERE user_id = ? AND symbol = ?", new_shares_tot, user_id, symbol)
                else:
                    # if user no longer has any shares of this stock then delete the row in holdings
                    db.execute("DELETE FROM holdings WHERE symbol = ?", symbol)

                # update users table
                # value of sale
                sale_value = sell_shares * price
                # find current user cash
                user_cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
                updated_cash = user_cash[0]["cash"] + sale_value
                # update table
                db.execute("UPDATE users SET cash = ? WHERE id = ?", updated_cash, user_id)

                # record sale in order_history
                db.execute("INSERT INTO order_history (user_id, symbol, shares, price, timestamp, bought_sold) VALUES (?, ?, ?, ?, ?, ?)",
                           user_id, symbol, sell_shares, price, datetime.now(), "sold")

                flash('Sold!')
                return redirect("/")
