from fourmen import app

if __name__ == '__main__':
    app.run(host='0.0.0.0')
    app.config['UPLOAD_FOLDER'] = './test/'
