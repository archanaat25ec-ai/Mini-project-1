from flask import Flask, render_template, request, redirect, session
import subprocess

app = Flask(__name__)
app.secret_key = "bankapp"

# LOGIN
@app.route('/', methods=['GET', 'POST'])
def login():
    if request.method == 'POST':
        if request.form['username'] == "admin" and request.form['password'] == "1234":
            session['user'] = "admin"
            return redirect('/dashboard')
        return "Invalid Login"
    return render_template('login.html')


# DASHBOARD
@app.route('/dashboard')
def dashboard():
    if 'user' not in session:
        return redirect('/')
    return render_template('dashboard.html')


# DISPLAY TABLE
@app.route('/accounts')
def accounts():
    output = subprocess.check_output(["./bank", "display"]).decode()
    
    rows = []
    for line in output.split('\n')[1:]:
        if line.strip():
            parts = line.split()
            rows.append(parts)

    return render_template('table.html', data=rows)


# SEARCH
@app.route('/search', methods=['POST'])
def search():
    acc = request.form['acc']
    output = subprocess.check_output(["./bank", "search", acc]).decode()
    return f"<pre>{output}</pre>"


# ADD
@app.route('/add', methods=['GET','POST'])
def add():
    if request.method == 'POST':
        subprocess.call([
            "./bank", "add",
            request.form['acc'],
            request.form['lname'],
            request.form['fname'],
            request.form['bal']
        ])
        return redirect('/accounts')
    return render_template('add.html')


# EDIT
@app.route('/edit/<acc>', methods=['GET','POST'])
def edit(acc):
    if request.method == 'POST':
        subprocess.call([
            "./bank", "update",
            acc,
            request.form['amount']
        ])
        return redirect('/accounts')
    return render_template('edit.html', acc=acc)


# DELETE
@app.route('/delete/<acc>')
def delete(acc):
    subprocess.call(["./bank", "delete", acc])
    return redirect('/accounts')


# DARK MODE TOGGLE
@app.route('/dark')
def dark():
    return render_template('dashboard.html', dark=True)

app.run(debug=True)