from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp
from passlib.context import CryptContext
from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

#-------------------------------------------------------------------------
@app.route("/", methods=["GET", "POST"])
@login_required
def index():
    """Index page with balance"""
    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # apply the buy/sell actions through index page
        return(index_apply())
    else:
        # refresh and show balance in index page
        balance = show_balance()
        return render_template("index.html",cash = balance["cash"],portfoliorows = balance["portfoliorows"], total = balance["total"])

#-------------------------------------------------------------------------
@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""

    if request.method == "POST":

        symbol = request.form.get("symbolbuy")      # get symbol of stock user wants to buy
        shares = request.form.get("sharesnr")       # get shares number user wants to buy

        # Check for forbiden characters
        if symbol_check(symbol) == True or symbol_check(shares) == True:
            return apology("Forbiden characters")

        if not symbol or not shares:                # check if the fields are not empty
            return apology("Fill up all data")
        elif lookup(symbol) == None:                # check if the symbol is valid and stock data can be retrieved
            return apology("Invalid symbol")
        elif int(shares) <= 0:                            # check if a share number is positive
            return apology("Invalid shares")
        else:
            return buy_shares(symbol,int(shares))   # make a purchase of the stock

    else:
        return render_template("buy.html")          # if its not reached by POST, generate initial buy page

#-------------------------------------------------------------------------
@app.route("/history")
@login_required
def history():
    """Show history of transactions."""

    # select the users purchase data from database and show it
    history = db.execute("SELECT symbol,shares,price,transacted FROM history WHERE id = :id", id=session["user_id"])
    return render_template("history.html",history=history)

#-------------------------------------------------------------------------
@app.route("/change_password", methods=["GET", "POST"])
@login_required
def change_password():
    """Change a password."""

    # get the user input
    currentpass = request.form.get("current_password")
    newpass1 = request.form.get("new_password_1")
    newpass2 = request.form.get("new_password_2")

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Check for forbiden characters
        if symbol_check(currentpass) == True or symbol_check(newpass1) == True  or symbol_check(newpass2) == True:
            return apology("Forbiden characters")

        # ensure current password was submitted
        if not currentpass:
            return apology("must provide a current password")

        # ensure a new password was submitted
        elif not newpass1:
            return apology("must provide a new password")

        # ensure a new password was confirmed
        elif not newpass2:
            return apology("must confirm a new password")

        # check if passwords do match
        if newpass1 != newpass2:
            return apology("passwords do not match")

        # limit password length
        elif len(newpass1) > 32 :
            return apology("Password is too long, max - 32 characters")

        # minimum password length
        elif len(newpass1) < 6 :
            return apology("Password is too short, at least 6 characters")

        else:
        # hash the password and insert a new user into the database
            return changepass(currentpass,newpass1)

    else:
        return render_template("change_password.html")      # initial change password page


#-------------------------------------------------------------------------
@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

#-------------------------------------------------------------------------
@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

#-------------------------------------------------------------------------
@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    symbol = request.form.get("symbol")     # get user input

    if request.method == "POST":

        if not symbol:                      # check if field is empty
            return apology("Enter the stock symbol to look up")
        elif lookup(symbol) == None:        # check if stock is valid and data can be retrieved
            return apology("Invalid symbol")
        else:
            # Check for forbiden characters
            if symbol_check(symbol) == True:
                return apology("Forbiden characters")
            result = lookup(symbol)         # retrieve and show stock's data
            return render_template("display.html", name = result['name'], symbol = result['symbol'], price = usd(result['price']))

    else:
        return render_template("quote.html")

#-------------------------------------------------------------------------
@app.route("/register", methods=["GET", "POST"])            # NO illegal symbols, spaces check                  #########
def register():
    """Register user."""

    # get user input
    username = request.form.get("username")
    password1 = request.form.get("password1")
    password2 = request.form.get("password2")

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not username:
            return apology("must provide username")

        # ensure password was submitted
        elif not password1:
            return apology("must provide password")

        # ensure password was confirmed
        elif not password2:
            return apology("must confirm password")

        # Check for forbiden characters
        if symbol_check(username) == True or symbol_check(password1) == True  or symbol_check(password2) == True:
            return apology("Forbiden characters")

        # limit username length
        elif len(username) > 32 :
            return apology("Username is too long")

        # limit password length
        elif len(password1) > 32 :
            return apology("Password is too long, max 32")

        # minimum password length
        elif len(password1) < 6 :
            return apology("Password is too short, at least 6 characters")

        # check if passwords do match
        if password1 != password2:
            return apology("passwords do not match")
        else:
        # hash the password and insert a new user into the database, a new user gets redirected to index
            return create_user(username,password1)

    else:
        return render_template("register.html")

#-------------------------------------------------------------------------
@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""

    if request.method == "POST":

        symbol = request.form.get("symbolsell")         # get user input
        shares = request.form.get("sharesnrsell")
        id = session["user_id"]

        # Check for forbiden characters
        if symbol_check(symbol) == True or symbol_check(shares) == True:
            return apology("Forbiden characters")
        if not symbol or not shares:                    # check if input is valid
            return apology("Fill up all data")
        elif lookup(symbol) == None:
            return apology("Invalid symbol")
        elif int(shares) < 0:
            return apology("Invalid shares")
        else:

            return sell_shares(symbol,int(shares))      # sell stocks

    else:
        return render_template("sell.html")

