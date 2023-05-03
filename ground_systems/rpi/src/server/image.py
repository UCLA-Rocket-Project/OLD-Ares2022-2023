from flask import Flask, send_file
app = Flask(__name__)

@app.route('/get_image')
def get_image():
    return send_file('img/pnid.png', mimetype='image/png')
