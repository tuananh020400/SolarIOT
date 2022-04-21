import 'package:flutter/material.dart';
import 'package:solar/MQTT/mqtt.dart';
import 'package:solar/MQTT/mqtt_app_state.dart';
import 'package:provider/provider.dart';
import 'package:lottie/lottie.dart';

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
      body: Padding(
        padding: EdgeInsets.only(left: 10,right: 10,top: 10,bottom: 0),
        child: Container(
            decoration: BoxDecoration(
              color: Color(0xFF292636),
              borderRadius: BorderRadius.circular(10),
            ),
            child: Column(
              mainAxisAlignment: MainAxisAlignment.center,
              crossAxisAlignment: CrossAxisAlignment.center,
              children: [
                Expanded(child: Lottie.asset(
                  'assets/benuoc.json',
                  controller: _controller,
                  onLoaded: (composition){
                    _controller.duration = composition.duration;
                    _controller.forward();
                    _controller.value = _mqtt.getAppState.getGardent1.getNhietDo.toDouble()/100;
                  },
                ),),
                Container(
                  width: 100,
                  color: Colors.white38,
                  child: Text('${_controller.value = _mqtt.getAppState.getGardent1.getNhietDo.toDouble()}%',
                    style: TextStyle(color: Colors.white, fontWeight: FontWeight.bold),),
                ),
              ],
            )
        ),
      ),
    );
  }
}