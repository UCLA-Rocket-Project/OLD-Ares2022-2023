import os
from flask import Flask,redirect

app = Flask(__name__)

dashboards = {
              "barracuda":"http://192.168.0.99:3000/d/XlDHbH-nk/barracuda?orgId=1&from=now-20s&to=now",
              "barracuda/p&id":"http://192.168.0.99:3000/d/_X8Kv0Y7k/barracuda-pandid?orgId=1&from=now-20s&to=now"
             }

@app.route('/')
def grafana_home():
    return redirect('http://192.168.0.99:3000/d/XlDHbH-nk/barracuda?orgId=1&from=now-20s&to=now', code=302)

@app.route('/<path:address>')
def grafana_dashboard(address):
    if address in dashboards:
        return redirect(dashboards[address], code=302)
    else:
        return redirect('http://192.168.0.99:3000/d/XlDHbH-nk/barracuda?orgId=1&from=now-20s&to=now', code=302)
