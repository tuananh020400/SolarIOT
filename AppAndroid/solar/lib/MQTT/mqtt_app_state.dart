import 'dart:convert';

import 'package:flutter/material.dart';
import 'package:flutter/cupertino.dart';

enum MQTTAppConnectionState {connected, disconnected, connecting}
class MQTTAppState with ChangeNotifier{
  MQTTAppConnectionState _appConnectionState = MQTTAppConnectionState.disconnected;
  String _receivedText = '';
  Garden _garden = Garden(
      nhietDo: 0,
      doAmDat: 0,
      doAm: 0,
      lightStatus: 0,
      fanStatus: 0,
      pumStatus: 0,
      lightButton: 0,
      pumpButton: 0,
      fanButton: 0,
      mode: 0
  );
  Garden _garden1 = Garden(
      nhietDo: 0,
      doAmDat: 0,
      doAm: 0,
      lightStatus: 0,
      fanStatus: 0,
      pumStatus: 0,
      lightButton: 0,
      pumpButton: 0,
      fanButton: 0,
      mode: 0
  );
  Gate _gate = Gate(docao: 0, chedo: 0, maybom: 0, maybomButton: 0);
  var _json;
  IconData _icon = Icons.cloud_off;
  String _connectionStringText = 'Disconnected';

  void setReceivedText(String text) {
    _receivedText = text;
    print(_receivedText);
    _json = jsonDecode(_receivedText);
  }
  void setGarden(){
    _garden = Garden(
        nhietDo: _json['nhietdo'],
        doAm: _json['doam'],
        doAmDat: _json['doamdat'],
        lightStatus: _json['light'],
        fanStatus: _json['fan'],
        pumStatus: _json['pump'],
        lightButton: _json['light'],
        pumpButton: _json['pump'],
        fanButton: _json['fan'],
        mode: _json['mode']
    );
    notifyListeners();
  }

  void setGate(){
    _gate = Gate(
      docao: _json['docao'],
      chedo: _json['chedo'],
      maybom: _json['maybom'],
      maybomButton: _json['maybom'],
    );
    _receivedText = '';
    notifyListeners();
  }

  void clearReceiveText(){
    _receivedText = '';
    notifyListeners();
  }

  void setAppConnectionState(MQTTAppConnectionState state){
    _appConnectionState = state;
    switch (state) {
      case MQTTAppConnectionState.connected:
        _icon = Icons.cloud_done;
        _connectionStringText = 'Connected';
        break;
      case MQTTAppConnectionState.disconnected:
        _icon = Icons.cloud_off;
        _connectionStringText = 'Disconnected';
        break;
      case MQTTAppConnectionState.connecting:
        _icon = Icons.cloud_upload;
        _connectionStringText = 'Connecting';
        break;
    }
    notifyListeners();
  }
  String get getReceivedText => _receivedText;
  MQTTAppConnectionState get getAppConnectionState => _appConnectionState;
  IconData get getIconData => _icon;
  dynamic get getConnectionStringText => _connectionStringText;
  Garden get getGardent => _garden;
  Garden get getGardent1 => _garden1;
  Gate get getGate => _gate;
}

class Garden{
  dynamic _nhietDo = 0;
  dynamic _doAm = 0;
  dynamic _doAmDat = 0;
  int _lightStatus = 0;
  int _fanStatus = 0;
  int _pumpStatus = 0;
  int _lightButton = 0;
  int _fanButton = 0;
  int _pumpButton = 0;
  int _mode = 0;
  Garden({
    required dynamic nhietDo,
    required dynamic doAm,
    required dynamic doAmDat,
    required int lightStatus,
    required int fanStatus,
    required int pumStatus,
    required int lightButton,
    required int fanButton,
    required int pumpButton,
    required int mode,
  }) :
        _nhietDo = nhietDo,
        _doAm = doAm,
        _doAmDat = doAmDat,
        _lightStatus = lightStatus,
        _pumpStatus = pumStatus,
        _fanStatus = fanStatus,
        _lightButton = lightButton,
        _fanButton = fanButton,
        _pumpButton = pumpButton,
        _mode = mode;

  void setLightButton(int lightButton){
    _lightButton = lightButton;
  }

  void setFanButton(int fanButton){
    _fanButton = fanButton;
  }

  void setPumpButton(int pumpButton){
    _pumpButton = pumpButton;
  }

  void setMode(int mode){
    _mode = mode;
  }

  dynamic get getNhietDo => _nhietDo;
  dynamic get getDoAm => _doAm;
  dynamic get getDoAmDat => _doAmDat;

  int get getLightStatus => _lightStatus;
  int get getFanStatus => _fanStatus;
  int get getPumpStatus => _pumpStatus;

  int get getLightButton => _lightButton;
  int get getFanButton => _fanButton;
  int get getPumpButton => _pumpButton;

  int get getMode => _mode;
}
class Gate{
  dynamic _docao;
  int _chedo;
  int _maybom;
  int _maybomButton;

  Gate({
    required dynamic docao,
    required int chedo,
    required int maybom,
    required int maybomButton,
  }):
        _docao = docao,
        _chedo = chedo,
        _maybom = maybom,
        _maybomButton = maybomButton;

  void setMayBomButton(int state){
    _maybomButton = state;
  }

  void setChedo(int chedo){
    _chedo = chedo;
  }

  dynamic get getDoCao => _docao;
  int get getCheDo => _chedo;
  int get getMayBom => _maybom;
  int get getMayBomButton => _maybomButton;
}