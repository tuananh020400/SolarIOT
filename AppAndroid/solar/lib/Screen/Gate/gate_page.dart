import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';
import 'package:percent_indicator/circular_percent_indicator.dart';
import 'package:percent_indicator/linear_percent_indicator.dart';
import 'package:solar/MQTT/mqtt.dart';
import 'package:solar/MQTT/mqtt_app_state.dart';
import 'package:provider/provider.dart';
import 'package:lottie/lottie.dart';
import 'package:solar/Package/animatedbutton.dart';
import 'dart:math';

class GatePage extends StatefulWidget{
  @override
  State<StatefulWidget> createState() {
    // TODO: implement createState
    return _GatePageState();
  }
}
class _GatePageState extends State<GatePage> with SingleTickerProviderStateMixin{
  late final AnimationController _controller;
  late MQTT _mqtt;
  late MQTTAppState _currentState;

  @override
  void initState() {
    // TODO: implement initState
    super.initState();
    _controller = AnimationController(vsync: this);
  }

  @override
  void dispose() {
    // TODO: implement dispose
    super.dispose();
    _controller.dispose();
  }

  @override
  Widget build(BuildContext context) {
    MQTT mqtt = Provider.of<MQTT>(context);
    _mqtt = mqtt;
    MQTTAppState currentState = Provider.of<MQTTAppState>(context);
    _currentState = currentState;
    return Scaffold(
      appBar: AppBar(
        centerTitle: true,
        backgroundColor: Color(0xFF292636),
        title: Text('GateWay',style: TextStyle(fontWeight: FontWeight.bold),),
        actions: <Widget>[
          Icon(_mqtt.getAppState.getIconData)
        ],
      ),
      body: Column(
        children: [
          Padding(padding: EdgeInsets.all(10)),
          _buildTank(),
          _buildMode(),
          _buildButton(),
        ],
      ),
    );
  }
  Widget _buildTank(){
    return Padding(
        padding: EdgeInsets.all(10),
        child: Container(
          height: 400,
          alignment: Alignment.topCenter,
          decoration: BoxDecoration(
            color: Color(0xFF292636),
            borderRadius: BorderRadius.circular(20),
          ),
          child: Padding(
            padding: EdgeInsets.only(bottom: 20,top: 0),
            child: Column(
              mainAxisAlignment: MainAxisAlignment.center,
              crossAxisAlignment: CrossAxisAlignment.center,
              children: [
                Expanded(
                    child: Transform.rotate(
                      angle: 3*pi / 2,
                      child: LinearPercentIndicator(
                        width: 300,
                        lineHeight: 150,
                        percent: _mqtt.getAppState.getGate.getDoCao/100,
                        linearStrokeCap: LinearStrokeCap.butt,
                        progressColor: Colors.blue,
                        center: Transform.rotate(
                          angle: pi/2,
                          child: Text('${_mqtt.getAppState.getGate.getDoCao}%',
                            style: TextStyle(
                                color: Color(0xFF292636),
                                fontSize: 25,fontWeight: FontWeight.bold
                            ),
                          ),
                        ),
                      ),
                    ),
                ),
                Padding(padding: EdgeInsets.all(40)),
                Container(
                  height: 50,
                  // padding: EdgeInsets.only(bottom: 20),
                  // alignment: Alignment.bottomCenter,
                    child: Column(
                      children: [
                        Text('Thể tích',style: TextStyle(color: Colors.white,fontWeight: FontWeight.bold,fontSize: 20),),
                        Padding(padding: EdgeInsets.all(1)),
                        Text('${_mqtt.getAppState.getGate.getDoCao*5/100}L/5L',style: TextStyle(color: Colors.white,fontWeight: FontWeight.bold,fontSize: 20),),
                      ],
                    ),
                )
              ],
            )
          )
        ),
    );
  }

  Widget _buildMode(){
    return AnimatedToggle(
      text: [' Auto', 'Manual '],
      buttonText: ['Auto', 'Manual'],
      onColor: Colors.blue,
      offColor: Colors.blue,
      backgroundColor: Color(0xFF292636),
      position: _mqtt.getAppState.getGate.getCheDo,
      onToggleCallback: (index) {
        setState(() {});
        _mqtt.getAppState.getGate.setChedo( _mqtt.getAppState.getGate.getCheDo == 1? 0 : 1);
        index == 1?
        _mqtt.getManager.publish("Manual Mode"):
        _mqtt.getManager.publish("Auto Mode");
      },
      width: MediaQuery.of(context).size.width,
      hight: 70,
    );
  }

  Widget _buildButton(){
    return Padding(
      padding: EdgeInsets.all(10),
      child: Container(
        decoration: BoxDecoration(
          color: Color(0xFF292636),
          borderRadius: BorderRadius.circular(10)
        ),
        child: Container(
          child: Row(
          children: [
            SizedBox(
              height: 100,
              child: Transform.rotate(
                  angle: pi/2,
                child: _mqtt.getAppState.getGate.getMayBom == 1?
                  Image.asset('assets/pumpon.png'):
                  Image.asset('assets/pump.png'),
              )
            )
          ],)
        ),
      ),
    );
  }
}