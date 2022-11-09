from flask import Flask, render_template, request,redirect,url_for
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
    led = db.Column(db.Boolean)
    sensor_movimiento = db.Column(db.Boolean)
    sensor_luminosidad = db.Column(db.Boolean)
    sensor_intensidad = db.Column(db.Integer)

    def __init__(self,luz_1):
        self.luz_1 = luz_1
        self.led = False
        self.sensor_luminosidad = False
        self.sensor_movimiento = False
        self.sensor_intensidad = 0

class DispositivosSchema(ma.SQLAlchemySchema):
    class Meta:
        model = Dispositivos
    _id = ma.auto_field()
    luz_1 = ma.auto_field()
    led = ma.auto_field()
    sensor_movimiento = ma.auto_field()
    sensor_luminosidad = ma.auto_field()
    sensor_intensidad = ma.auto_field()


@app.route("/")
def hello_world():
    dsp =  Dispositivos.query.filter(Dispositivos._id == 1).first()
    print(dsp.sensor_intensidad)  
    return render_template("home.html",disp=dsp,intensidad=dsp.sensor_intensidad)

@app.route("/test")
def test():
    dsp = Dispositivos.query.filter(Dispositivos._id == 1).first()
    app.logger.info('dispositivos: %s',dsp)
    disp_schema = DispositivosSchema()
    dump = disp_schema.dump(dsp)
    return dump

@app.route('/handle_data', methods=['POST'])
def handle_data():
    dispositivos_validos = ['luz_1','led','sensor_movimiento','sensor_luminosidad','sensor_intensidad']
    checkValues(dispositivos_validos,request.form)
    return redirect(url_for('hello_world'))

def checkValues(dispositivos_validos,form):
    dato_base =  Dispositivos.query.filter(Dispositivos._id == 1).first()
    try:
        for d in dispositivos_validos:
            if d=='sensor_intensidad':
                setattr(dato_base, d,form.to_dict()['intensidad'])
            else:
                if d in form:
                    setattr(dato_base, d,True)
                else:
                    setattr(dato_base, d,False)
        db.session.commit()
    except Exception:
        pass 
    

@app.before_first_request
def create_tables():
    db.create_all()
    dispositivo = Dispositivos(False)
    db.session.add(dispositivo)
    db.session.commit()


if __name__ == "__main__":
    app.run(debug=True)
