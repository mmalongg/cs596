app = Flask(__name__)

@app.route("/")
def hello():
    value = request.args.get("var", default="", type=str)
    print(value)

    # Store the value in a text file
    with open("log.txt", "a") as f:

        f.write(value + "\n")

    return "We received value: " + value

if __name__ == "__main__":
    app.run(debug=True)