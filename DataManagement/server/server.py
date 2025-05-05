from flask import Flask, request, jsonify
from DatabaseHandler import DatabaseHandler

app = Flask(__name__)
db = DatabaseHandler("datas.db")

@app.route('/add_data', methods=['POST'])
def add_data():
    data = request.json
    try:
        db.add_data(
            data['temperature'],
            data['humidity'],
            data['gas'],
            data['steam'],
            data['date'],
            data['hour']
        )
        return jsonify({"status": "success"}), 200
    except Exception as e:
        return jsonify({"status": "error", "message": str(e)}), 400

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)