import 'dart:ffi';
import 'package:flutter/material.dart';
import 'package:syncfusion_flutter_charts/charts.dart';
import 'dart:async';

class Chart extends StatefulWidget{
  double data;
  Chart({Key? key, required this.data}) : super(key: key);

  @override
  State<StatefulWidget> createState() {
    // TODO: implement createState
    return _Chart();
  }
}

class _Chart extends State<Chart>{
  late List<LiveData> chartData;
  late ChartSeriesController _chartSeriesController;

  @override
  void initState() {
    chartData = getChartData();
    Timer.periodic(const Duration(seconds: 1), updateDataSource);
    super.initState();
  }

  @override
  Widget build(BuildContext context) {
    // TODO: implement build
    return SfCartesianChart(
      title: ChartTitle(text: '${widget.data} °C',textStyle: TextStyle(color: Colors.red,),alignment: ChartAlignment.far),
      tooltipBehavior: TooltipBehavior(enable: true),
      series: <LineSeries<LiveData, int>>[
        LineSeries<LiveData, int>(
          onRendererCreated: (ChartSeriesController controller){
            _chartSeriesController = controller;
          },
          dataSource: chartData,
          color: Colors.red,
          isVisible: true,
          xValueMapper: (LiveData sales, _) => sales.time,
          yValueMapper: (LiveData sales, _) => sales.chartData,
        ),
      ],
      primaryXAxis: NumericAxis(
        majorGridLines:  const MajorGridLines(width: 0),
        edgeLabelPlacement: EdgeLabelPlacement.shift,
        interval: 3,
        title: AxisTitle(text: 'Thời gian (s)', textStyle: TextStyle(color: Colors.blue,)),
      ),
      primaryYAxis: NumericAxis(
          axisLine: const AxisLine(width: 0),
          majorTickLines: const MajorTickLines(size: 0),
          title: AxisTitle(text: 'Nhiệt độ (°C)',textStyle: TextStyle(color: Colors.blue,))
      ),
    );
  }

  int time = 19;
  void updateDataSource(Timer timer) {
    chartData.add(LiveData(time++, (widget.data.round())));
    chartData.removeAt(0);
    _chartSeriesController.updateDataSource(
        addedDataIndex: chartData.length - 1, removedDataIndex: 0);
  }

  List<LiveData> getChartData() {
    return <LiveData>[
      LiveData(0, 0),
      LiveData(1, 0),
      LiveData(2, 0),
      LiveData(3, 0),
      LiveData(4, 0),
      LiveData(5, 0),
      LiveData(6, 0),
      LiveData(7, 0),
      LiveData(8, 0),
      LiveData(9, 0),
      LiveData(11, 0),
      LiveData(12, 0),
      LiveData(13, 0),
      LiveData(14, 0),
      LiveData(15, 0),
      LiveData(16, 0),
      LiveData(17, 0),
      LiveData(18, 0),

    ];
  }
}

class LiveData {
  final int time;
  final num chartData;
  LiveData(this.time, this.chartData);

}