import csv
import urllib.request

from flask import redirect, render_template, request, session, url_for
from functools import wraps

from passlib.apps import custom_app_context as pwd_context
from passlib.context import CryptContext
from cs50 import SQL

db = SQL("sqlite:///finance.db")

def apology(top="", bottom=""):
    """Renders message as an apology to user."""
    def escape(s):
        """
        Escape special characters.

        https://github.com/jacebrowning/memegen#special-characters
        """
        for old, new in [("-", "--"), (" ", "-"), ("_", "__"), ("?", "~q"),
            ("%", "~p"), ("#", "~h"), ("/", "~s"), ("\"", "''")]:
            s = s.replace(old, new)
        return s
    return render_template("apology.html", top=escape(top), bottom=escape(bottom))

#-------------------------------------------------------------------------
def login_required(f):
    """
    Decorate routes to require login.

    http://flask.pocoo.org/docs/0.11/patterns/viewdecorators/
    """
    @wraps(f)
    def decorated_function(*args, **kwargs):
        if session.get("user_id") is None:
            return redirect(url_for("login", next=request.url))
        return f(*args, **kwargs)
    return decorated_function

#-------------------------------------------------------------------------
def lookup(symbol):
    """Look up quote for symbol."""

    # reject symbol if it starts with caret
    if symbol.startswith("^"):
        return None

    # reject symbol if it contains comma
    if "," in symbol:
        return None


    # query Yahoo for quote
    # http://stackoverflow.com/a/21351911
    try:
        url = "http://download.finance.yahoo.com/d/quotes.csv?f=snl1&s={}".format(symbol)
        webpage = urllib.request.urlopen(url)
        datareader = csv.reader(webpage.read().decode("utf-8").splitlines())
        row = next(datareader)
    except:
        return None

    # ensure stock exists
    try:
        price = float(row[2])
    except:
        return None

    # return stock's name (as a str), price (as a float), and (uppercased) symbol (as a str)
    return {
        "name": row[1],
        "price": price,
        "symbol": row[0].upper()
    }

#-------------------------------------------------------------------------
def usd(value):
    """Formats value as USD."""
    return "${:,.2f}".format(value)

#-------------------------------------------------------------------------
def money(value):
    """Formats value as money - use only one separator between full dolar and cents."""
    float(value)
    return "{:.2f}".format(value)

#-------------------------------------------------------------------------
def create_user(username,password1):
    """Hashes the password and inserts a new user into database"""

    # define hashing parameters
    hasher = CryptContext(schemes=["sha256_crypt"])

    # hash the user password
    hash1 = hasher.hash(password1)

    # check if the username is not already taken
    rows = db.execute("SELECT * FROM users WHERE username = :username", username=username)
    if len(rows) == 1:
        return apology("username already exists")
    else:
        # insert a new user to the database and redirect to the index page with the current balance
        db.execute("INSERT INTO users (username,hash) VALUES(:username, :hash)", username=username, hash=hash1)
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))
        session["user_id"] = rows[0]["id"]
        balance = show_balance()
        return render_template("index.html",cash = balance["cash"],portfoliorows = balance["portfoliorows"], total = balance["total"],
        title = '<div class="alert alert-success" role="alert">  A new user was created successfuly!        </div>')

#-------------------------------------------------------------------------
def changepass(currentpass,newpass1):
    """Hashes the password and inserts into database"""

    # check if the current password is valid
    id=session["user_id"]
    passwordrow = db.execute("SELECT hash FROM users WHERE id = :id", id=id)
    if len(passwordrow) != 1 or not pwd_context.verify(currentpass, passwordrow[0]["hash"]):
            return apology("invalid password")

    # hash the new password
    hasher = CryptContext(schemes=["sha256_crypt"])
    hash1 = hasher.hash(newpass1)

    # update the pasword
    db.execute("UPDATE users SET hash = :hash WHERE id = :id", hash=hash1, id=id)

    # refresh the balance and redirect to index
    balance = show_balance()
    return render_template("index.html",cash = balance["cash"],portfoliorows = balance["portfoliorows"], total = balance["total"],
    title = '<div class="alert alert-success" role="alert">  Password was changed!                    </div>')

#-------------------------------------------------------------------------
def show_balance():

    id=session["user_id"]

    # count the value of all stocks owned by the user
    sharesvalue = 0

    # get the cash of the user
    rows = db.execute("SELECT * FROM users WHERE id = :id", id=id)
    cash = rows[0]["cash"]

    #refresh prices of the stocks
    update_prices()

    # iterate trough the rows of user portfolio, one row for every stock
    portfoliorows = db.execute("SELECT symbol,name,shares,price,total FROM stocks WHERE id = :id", id=id)

    # if there is any rows
    prowcount = len(portfoliorows)
    if len(portfoliorows) > 0 :

        # for every stock, add its value to the total stock value
        for portfoliorow in portfoliorows:
            prowcount -= 1  # row number
            sharesvalue = sharesvalue + portfoliorows[prowcount]["total"]

    # total portfolio value
    total = cash + sharesvalue

    return {
        "cash": usd(rows[0]["cash"]),
        "portfoliorows": portfoliorows,
        "total": usd(total)
    }

#-------------------------------------------------------------------------
def update_prices():


    id=session["user_id"]

    # get the list of stocks owned by the user
    stocklist = db.execute("SELECT symbol,shares,price FROM stocks WHERE id = :id", id=id)
    stocklistnr = len(stocklist)
    if len(stocklist) > 0:
        for stock in stocklist:

            # get an updated data of the stock
            stocklistnr -= 1
            symbol = stock["symbol"]
            result = lookup(symbol)
            if result == None:          # if a valid data cannot be retrived, exit price update
                return
            else:

                # if a new data is diferent than the old one, update it
                newprice = float(result["price"])
                if newprice != stock["price"]:
                    newtotal = stock["shares"] * newprice
                    db.execute("UPDATE stocks SET price = :newprice, total = :newtotal WHERE id = :id AND symbol = :symbol",
                    newprice=money(newprice), newtotal=money(newtotal),id=id, symbol=symbol)

#-------------------------------------------------------------------------
def buy_shares(symbol,shares):

    id = session["user_id"]
    result = lookup(symbol)         # get stock's data
    totalbuy = result['price'] * shares     #the total amout of money needed for purchase

    cashrow = (db.execute("SELECT cash FROM users WHERE id = :id", id=id))  # get users cash
    cash = cashrow[0]["cash"]

    symbol = result['symbol']
    price = result['price']
    name = result['name']

    # check if the user has enough funds to make a purchase
    if cash < totalbuy:
        return apology("Not enough cash")
    else:

        # if a user already has the same stock, update the shares number a user owns
        initialshares = db.execute("SELECT shares FROM stocks WHERE id = :id AND symbol = :symbol", id=id, symbol=symbol)
        if len(initialshares) == 1 :
            totalshares = initialshares[0]["shares"] + shares
            db.execute("UPDATE stocks SET shares = :new, total = :newtotal WHERE id = :id AND symbol = :symbol",
            new=totalshares, newtotal = money(price*totalshares), id=id, symbol=symbol)

        else:

            # if it's a new stock, insert it into user portfolio
            db.execute("INSERT INTO stocks (id, symbol, name, shares, price, total) VALUES(:id, :symbol, :name, :shares, :price, :total)",
            id=id, symbol=symbol,name=name,shares=shares,price=money(price), total=money(totalbuy))

        # insert log into HISTORY
        db.execute("INSERT INTO history (id, symbol, shares, price) VALUES(:id, :symbol, :shares, :price)",
        id=id, symbol=symbol,shares=shares,price=money(price))

        # update user cash after purchase
        db.execute("UPDATE users SET cash = :new WHERE id = :id", new=money(cash-totalbuy), id=id)

        # show balance
        balance = show_balance()
        return render_template("index.html",cash = balance["cash"],portfoliorows = balance["portfoliorows"], total = balance["total"],
        title = '<div class="alert alert-success" role="alert">  Bought!                    </div>')

#-------------------------------------------------------------------------
def sell_shares(symbol, shares):

    id = session["user_id"]
    result = lookup(symbol)
    totalsell = result['price'] * shares        # amount of money to be earned by selling stocks

    cashrow = (db.execute("SELECT cash FROM users WHERE id = :id", id=id))  # get current cash of the user
    cash = cashrow[0]["cash"]

    symbol = result['symbol']
    price = result['price']
    name = result['name']

    # check if the user has this share and enough of them to sell
    initialshares = db.execute("SELECT shares FROM stocks WHERE id = :id AND symbol = :symbol", id=id, symbol=symbol)

    if len(initialshares) != 1 :
        return apology("You don't have shares of this name")

    if initialshares[0]["shares"] < shares:
        return apology("Not enough shares")
    else:
        if len(initialshares) == 1 :
            totalshares = initialshares[0]["shares"] - shares       # get the new number of shares a user will own

            # if no shares left - delete a stock from user's portfolio
            if totalshares == 0:
                db.execute("DELETE FROM stocks WHERE id=:id AND symbol=:symbol",
                id=id, symbol=symbol)
            else:

                # update the shares number
                db.execute("UPDATE stocks SET shares=:new, price=:price, total=:newtotal WHERE id=:id AND symbol=:symbol",
                new=totalshares, price=price, id=id, newtotal=money(price * totalshares), symbol=symbol)

        # insert log into HISTORY
        db.execute("INSERT INTO history (id, symbol, shares, price) VALUES(:id, :symbol, :shares, :price)",
        id=id, symbol=symbol,shares=-shares,price=money(price))

        #update user's cash
        db.execute("UPDATE users SET cash = :new WHERE id = :id", new=money(cash+totalsell), id=id)

        # show balance
        balance = show_balance()
        return render_template("index.html",cash = balance["cash"],portfoliorows = balance["portfoliorows"], total = balance["total"],
            title = '<div class="alert alert-success" role="alert">  Sold!                    </div>')

#-------------------------------------------------------------------------
def index_apply():      # make stock purchases from index page


    id = session["user_id"]

    char_trigger = 0    # number of "forbidden character" errors
    shares_trigger = 0  # number of "not enough shares" errors
    cash_trigger = 0    # number of "not enough cash" errors
    err_msg = ''        # text of error message

    # iterate trough the user stocks
    portfolio = db.execute("SELECT symbol FROM stocks WHERE id = :id", id=id)

    # if there is any stocks
    portfolionr = len(portfolio)
    if portfolionr > 0:
        for row in portfolio:

            if symbol_check(request.form.get(row["symbol"])) != True:

                # get user input from the specified shares row
                shares_count = int(request.form.get(row["symbol"]))

                # if it's negative - perform sell operation
                if shares_count < 0:
                    err_check = sell_shares(row["symbol"],-int(shares_count))

                    # if sell operation returned an error increase error counter and form an error message
                    if err_check == apology("Not enough shares"):
                        shares_trigger += 1
                        err_msg = err_msg + " " + row["symbol"]

                # if it's positive - perform buy operation
                elif shares_count > 0:
                    err_check = buy_shares(row["symbol"],int(shares_count))

                    # if buy operation returned an error increase error counter and form an error message
                    if err_check == apology("Not enough cash"):
                        cash_trigger += 1
                        err_msg = err_msg + " " + row["symbol"]
            else:
                char_trigger += 1
                err_msg = err_msg + " " + row["symbol"]

    # set standard message
    title = '<div class="alert alert-success" role="alert">  Actions performed!                    </div>'

    # if any errors ocoured, update message with error message
    if cash_trigger > 0 or shares_trigger > 0:
        title = '<div class="alert alert-danger" role="alert">Not enouch shares or cash on:' + err_msg + '                    </div>'
    # if forbidden character error, update message with error message
    if char_trigger > 0:
        title = '<div class="alert alert-danger" role="alert">forbidden characters:' + err_msg + '                    </div>'
    # show balance
    balance = show_balance()
    return render_template("index.html",cash = balance["cash"],portfoliorows = balance["portfoliorows"], total = balance["total"],
    title = title)

#-------------------------------------------------------------------------
def symbol_check(symbol):      # check for incorrect symbols / no optimization

    if "," in symbol or "." in symbol or "'" in symbol or ";" in symbol or "/" in symbol:
        return True
