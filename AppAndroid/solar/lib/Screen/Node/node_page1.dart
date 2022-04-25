import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:solar/Package/chart.dart';
import 'package:solar/MQTT/mqtt.dart';
import 'package:solar/MQTT/mqtt_app_state.dart';
import 'package:solar/Package/animatedbutton.dart';
import 'package:lottie/lottie.dart';

class NodePage1 extends StatefulWidget{
  @override
  _NodePageState1 createState()  => _NodePageState1();
}

class _NodePageState1 extends State<NodePage1> with TickerProviderStateMixin{
  late MQTT _mqtt;
  late MQTTAppState _currentState;
  late final AnimationController _lightController;
  late final AnimationController _fanController;
  late final AnimationController _pumpController;

  @override
  void initState() {
    // TODO: implement initState
    super.initState();
    _fanController = AnimationController(vsync: this);
    _lightController = AnimationController(vsync: this);
    _pumpController = AnimationController(vsync: this);
  }

  @override
  Widget build(BuildContext context) {
    final MQTT mqtt = Provider.of<MQTT>(context);
    _mqtt = mqtt;
    final MQTTAppState currentState = Provider.of<MQTTAppState>(context);
    _currentState = currentState;
    return Scaffold(
        appBar: AppBar(
            centerTitle: true,
            backgroundColor: Color(0xFF292639),
            title: const Text('Vườn 2'),
            actions: <Widget>[
              Icon(_mqtt.getAppState.getIconData)
            ]),
        body: SingleChildScrollView(
          child: Column(
            children: <Widget>[
              Padding(padding: EdgeInsets.all(10)),
              _buildTabView(),
              _buildAuto(),
              _buildButton()
            ],
          ),
        )
    );
  }

  Widget _buildTabView(){
    return DefaultTabController(
        length: 3,
        child: Padding(
            padding: const EdgeInsets.symmetric(vertical: 0, horizontal: 20),
            child: Column(
              children: [
                Container(
                    decoration: BoxDecoration(
                        color: Color(0xFF292639),
                        borderRadius: BorderRadius.circular(30)
                    ),
                    child: Padding(
                      padding: const EdgeInsets.all(5),
                      child: TabBar(
                          indicator: BoxDecoration(
                              color: Colors.blue,
                              borderRadius: BorderRadius.circular(30)),
                          tabs: [
                            Tab(child: Text('Nhiệt độ',style: TextStyle(fontWeight: FontWeight.bold),),),
                            Tab(child: Text('Độ ẩm',style: TextStyle(fontWeight: FontWeight.bold),),),
                            Tab(child: Text('Độ ẩm đất',style: TextStyle(fontWeight: FontWeight.bold),),),
                          ]),
                    )
                ),
                SizedBox(height: 20,),
                Container(
                    decoration: BoxDecoration(
                      borderRadius: BorderRadius.circular(10),
                      color: Color(0xFF292639),
                    ),
                    child: SizedBox(
                      height: 300,
                      child: TabBarView(
                        children: [
                          _buildNhietDo(),
                          _buildDoAm(),
                          _buildDoAmDat(),
                        ],
                      ),
                    )
                )
              ],
            )
        )
    );
  }

  Widget _buildAuto(){
    return AnimatedToggle(
      text: [' Auto', 'Manual '],
      buttonText: ['Auto', 'Manual'],
      onColor: Colors.blue,
      offColor: Colors.blue,
      backgroundColor: Color(0xFF292636),
      position: _mqtt.getAppState.getGardent1.getMode,
      onToggleCallback: (index) {
        setState(() {});
        _mqtt.getAppState.getGardent1.setMode( _mqtt.getAppState.getGardent1.getMode == 1? 0 : 1);
        index == 1?
        _mqtt.getManager.publish("H1I"):
        _mqtt.getManager.publish("H0I");
      },
      width: MediaQuery.of(context).size.width,
      hight: 70,
    );
  }

  Widget _buildNhietDo(){
    return Chart(data: _mqtt.getAppState.getGardent1.getNhietDo.toDouble());
  }

  Widget _buildDoAm() {
    return Container(
      height: 100,
      child: Stack(
        children: [
          Center(
            child: Lottie.asset('assets/water.json'),
          ),
          Center(
            child: Lottie.asset('assets/in.json'),
          ),
          Center(
            child: Text('${_mqtt.getAppState.getGardent1.getDoAm}%',
              style: TextStyle(
                color: Color(0xFF0D3770),
                fontSize: 50,
                fontWeight: FontWeight.bold,
              ),
            ),
          )
        ],
      ),
    );
  }

  Widget _buildDoAmDat(){
    return Container(
      height: 100,
      child: Stack(
        children: [
          Center(
            child: Lottie.asset('assets/out.json'),
          ),
          Center(
            child: Lottie.asset('assets/in.json'),
          ),
          Center(
            child: Text('${_mqtt.getAppState.getGardent1.getDoAmDat}%',
              style: TextStyle(
                color: Color(0xFF0D3770),
                fontSize: 50,
                fontWeight: FontWeight.bold,
              ),
            ),
          )
        ],
      ),
    );
  }

  Widget _buildButton(){
    return Padding(
      padding: EdgeInsets.symmetric(horizontal: 20,vertical: 10),
      child: Container(
        height: 150,
        child: Row(
          children: [
            Expanded(
                child: Container(
                  decoration: BoxDecoration(
                    borderRadius: BorderRadius.circular(10),
                    color: Color(0xFF292636),
                  ),
                  child: Column(
                    children: [
                      Expanded(
                        child: SizedBox(
                          height: 50,
                          width: 60,
                          child: _mqtt.getAppState.getGardent1.getFanStatus == 1?
                          Lottie.asset(
                            'assets/fan.json',
                          ) :
                          Lottie.asset(
                              'assets/fanoff.json',
                              repeat: false,
                              controller: _fanController,
                              onLoaded: (composition){
                                _fanController.duration = composition.duration;
                                _fanController.forward();
                                _fanController.value = 0;
                              }
                          ),
                        ),
                      ),
                      _mqtt.getAppState.getGardent1.getMode == 1?
                      Padding(
                        padding: const EdgeInsets.only(bottom: 20),
                        child: CupertinoSwitch(
                            value: _mqtt.getAppState.getGardent1.getFanButton == 1? true : false,
                            onChanged: (index){
                              setState(() {});
                              _mqtt.getAppState.getGardent1.setFanButton(_mqtt.getAppState.getGardent1.getFanButton == 1? 0:1);
                              index == true?
                              _mqtt.getManager.publish('F1G'):
                              _mqtt.getManager.publish('F0G');
                            }),
                      ) : SizedBox.shrink(),
                    ],
                  ),
                )),
            SizedBox(width: 5,),
            Expanded(
                child: Container(
                  decoration: BoxDecoration(
                    borderRadius: BorderRadius.circular(10),
                    color: Color(0xFF292636),
                  ),
                  child: Column(
                    children: [
                      Expanded(child: _mqtt.getAppState.getGardent1.getLightStatus == 1?
                      Lottie.asset(
                          'assets/light.json',
                          controller: _lightController,
                          repeat: true,
                          onLoaded: (composition){
                            _lightController.duration = composition.duration;
                            _lightController.forward();
                          }
                      ) :
                      Lottie.asset(
                          'assets/lightoff.json',
                          repeat: false
                      )),
                      _mqtt.getAppState.getGardent1.getMode == 1?
                      Padding(
                        padding: const EdgeInsets.only(bottom: 20),
                        child: CupertinoSwitch(
                          value: _mqtt.getAppState.getGardent1.getLightButton == 1? true:false,
                          onChanged: (index){
                            setState(() {});
                            _mqtt.getAppState.getGardent1.setLightButton(_mqtt.getAppState.getGardent1.getLightButton == 1? 0:1);
                            index == true?
                            _mqtt.getManager.publish('G1H'):
                            _mqtt.getManager.publish('G0H');
                          },
                        ),
                      ) : SizedBox.shrink()
                    ],
                  ),
                )),
            SizedBox(width: 5,),
            Expanded(
                child: Container(
                  decoration: BoxDecoration(
                    borderRadius: BorderRadius.circular(10),
                    color: Color(0xFF292636),
                  ),
                  child: Column(
                    children: [
                      Expanded(child: _mqtt.getAppState.getGardent1.getPumpStatus == 1?
                      Lottie.asset(
                          'assets/binhnuoctuoicay.json'
                      ):
                      Padding(
                        padding: EdgeInsets.only(left: 35),
                        child: Lottie.asset(
                            'assets/pumpoff.json',
                            controller: _pumpController,
                            onLoaded: (composition){
                              _pumpController.duration = composition.duration;
                              _pumpController.forward();
                              _pumpController.value = 0;
                            }
                        )
                        ,)
                      ),
                      _mqtt.getAppState.getGardent1.getMode == 1?
                      Padding(
                        padding: const EdgeInsets.only(bottom: 20),
                        child: CupertinoSwitch(
                            value: _mqtt.getAppState.getGardent1.getPumpButton == 1? true : false,
                            onChanged: (index){
                              setState(() {});
                              _mqtt.getAppState.getGardent1.setPumpButton(_mqtt.getAppState.getGardent1.getPumpButton == 1? 0 : 1);
                              index == true?
                              _mqtt.getManager.publish('E1F'):
                              _mqtt.getManager.publish('E0F');
                            }
                        ),
                      ): SizedBox.shrink(),
                    ],
                  ),
                )),
          ],
        ),
      ),
    );
  }

}