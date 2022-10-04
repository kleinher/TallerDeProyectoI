from flask import Flask, render_template, request
from flask_sqlalchemy import SQLAlchemy
from flask_marshmallow import Marshmallow

app = Flask(__name__)

app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///dispositivos.sqlite3'
app.config["SQLALCHEMY_TRACK_MODIFICATIONS"] = False

db = SQLAlchemy(app)
ma = Marshmallow(app)
class Dispositivos(db.Model):
    _id = db.Column("id",db.Integer,primary_key=True)
    luz_1 = db.Column(db.Boolean)
    luz_2 = db.Column(db.Boolean)
    luz_3 = db.Column(db.Boolean)
    luz_4 = db.Column(db.Boolean)
    sensor_movimiento = db.Column(db.Boolean)
    sensor_luminosidad = db.Column(db.Boolean)
    sensor_intensidad = db.Column(db.Boolean)

    def __init__(self,luz_1):
        self.luz_1 = luz_1
        self.luz_2 = False
        self.luz_3 = False
        self.luz_4 = False
        self.sensor_intensidad = False
        self.sensor_luminosidad = False
        self.sensor_movimiento = False

class DispositivosSchema(ma.SQLAlchemySchema):
    class Meta:
        model = Dispositivos

#@app.route("/test")
#def test():
#    test =  users.query.filter_by(name="hernan").first()
#    test.email = "test"
#    db.session.commit()
#    return f"<p>nombre:{test.name}, apellido:{test.email}</p>"

@app.route("/")
def hello_world():
    dsp =  Dispositivos.query.first()
    return render_template("home.html",disp=dsp)
@app.route("/test")
def test():
    dsp = Dispositivos.query.first()
    disp_schema = DispositivosSchema()
    return disp_schema.dump(dsp)

@app.route('/handle_data', methods=['POST'])
def handle_data():
    dsp =  Dispositivos.query.first()
    try:
        luz_1 = request.form['luz_1']
        if luz_1 == 'on':
            dsp.luz_1 = True
        else:
            dsp.luz_1 = False
        db.session.commit()
    except:
        pass
    return render_template("home.html",disp=dsp)


@app.before_first_request
def create_tables():
    db.create_all()
    dispositivo = Dispositivos(False)
    db.session.add(dispositivo)
    db.session.commit()


if __name__ == "__main__":
    app.run(debug=True)
