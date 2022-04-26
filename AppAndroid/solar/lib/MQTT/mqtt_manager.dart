import 'package:solar/MQTT/mqtt_app_state.dart';
import 'package:mqtt_client/mqtt_client.dart';
import 'package:mqtt_client/mqtt_server_client.dart';

class MQTTManager{
  final MQTTAppState _currentState;
  MqttServerClient? _client;
  final String _identifier;
  final String _host;
  final String _topicpub;
  final String _topicsub;

  MQTTManager({
    required String host,
    required String topicpub,
    required String topicsub,
    required String identifier,
    required MQTTAppState state}):
        _identifier = identifier,
        _host = host,
        _topicpub = topicpub,
        _topicsub = topicsub,
        _currentState = state;

  void initializeMQTTClient(){
    _client = MqttServerClient(_host, _identifier);
    _client!.port = 1883;
    _client!.keepAlivePeriod = 20;
    _client!.onDisconnected = onDisconnected;
    _client!.secure = false;
    _client!.logging(on: true);

    _client!.onConnected = onConnected;
    _client!.onSubscribed = onSubscribed;
  }

  void connect() async {
    assert(_client != null);
    try {
      print("Start client connecting...");
      _currentState.setAppConnectionState(MQTTAppConnectionState.connecting);
      await _client!.connect();
    } on Exception catch (e){
      print('Ngat ket noi');
      disconnect();
    }
  }

  void disconnect(){
    print('Disconnect');
    _client!.disconnect();
  }

  void publish(String message){
    final MqttClientPayloadBuilder builder = MqttClientPayloadBuilder();
    builder.addString(message);
    _client!.publishMessage(_topicpub, MqttQos.exactlyOnce, builder.payload!);
  }

  void onSubscribed(String topic)
  {
    print('Subscribed comfirmed for topic $_topicpub');
  }

  void onDisconnected(){
    _currentState.setAppConnectionState(MQTTAppConnectionState.disconnected);
  }

  void onConnected(){
    _currentState.setAppConnectionState(MQTTAppConnectionState.connected);
    print('Client connected');
    _client!.subscribe(_topicsub, MqttQos.atLeastOnce);
    _client!.updates!.listen((List<MqttReceivedMessage<MqttMessage?>>? c) {
      final MqttPublishMessage recMess = c![0].payload as MqttPublishMessage;
      late final String pt;
      pt = MqttPublishPayload.bytesToStringAsString(recMess.payload.message);
      _currentState.setReceivedText(pt);
      _currentState.setGarden();
      _currentState.setGarden1();
      _currentState.setGate();
    });
  }
}