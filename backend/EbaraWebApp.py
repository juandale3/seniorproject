from flask import Flask
webapp = Flask(__name__)



@webapp.route('/')
def root():
    return 'Root Page loaded'







if __name__ == '__main__':
    webapp.run(debug=True, port = 8080)