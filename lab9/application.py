import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")

@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "POST":

        # TODO: Add the user's entry into the database
        name = request.form.get("name")
        month = request.form.get("month")
        day = request.form.get("day")
        name2 = request.form.get("name2")
        id2 = request.form.get("id")
        newName = request.form.get("new_name")
        id3 = request.form.get("id3")
        month2 = request.form.get("month2")
        day2 = request.form.get("day2")
        id4 = request.form.get("id4")

        if name2:
            db.execute("DELETE FROM birthdays WHERE name = ?", name2)
            return redirect("/")
        if id2:
            db.execute("DELETE FROM birthdays WHERE id = ?", id2)
            return redirect("/")
        if newName:
            db.execute("UPDATE birthdays SET name = ? WHERE id = ?", newName, id3)
            return redirect("/")
        if id4:
            db.execute("UPDATE birthdays SET month = ?, day = ? WHERE id = ?", month2, day2, id4)
            return redirect("/")

        db.execute("INSERT INTO birthdays (name, month, day) VALUES(?, ?, ?)", name, month, day)

        return redirect("/")

    else:

        # TODO: Display the entries in the database on index.html

        birthdays = db.execute("SELECT * FROM birthdays")

        # on the left is the name used in index.html
        # on the right is the python variable
        return render_template("index.html", birthdays = birthdays)


@app.route("/delete", methods=["GET", "POST"])
def delete():
    if request.method == "POST":
        name = request.form.get("name2")

        if name:
            db.execute("DELETE FROM birthdays WHERE name = ?", name)

        return redirect("/delete")

    else:

        # TODO: Display the entries in the database on index.html

        birthdays = db.execute("SELECT * FROM birthdays")

        return render_template("delete.html", birthdays = birthdays)





