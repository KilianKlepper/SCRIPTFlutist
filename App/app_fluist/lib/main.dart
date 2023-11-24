import 'dart:async';
import 'dart:typed_data';
import 'dart:io' show Platform;

import 'package:location_permissions/location_permissions.dart';
import 'package:flutter/material.dart';
import 'package:flutter_reactive_ble/flutter_reactive_ble.dart';
import 'package:flutter_colorpicker/flutter_colorpicker.dart';

void main() {
  return runApp(
    const MaterialApp(debugShowCheckedModeBanner: false, home: HomePage()),
  );
}

class HomePage extends StatefulWidget {
  const HomePage({Key? key}) : super(key: key);

  @override
  _HomePageState createState() => _HomePageState();
}

class _HomePageState extends State<HomePage> {
// App Header String
  String connectionText = "";

// ARGB
  int alpha = 0;
  int red = 0;
  int blue = 0;
  int green = 0;
  int effect = 0;
// Some state management stuff
  bool _foundDeviceWaitingToConnect = false;
  bool _scanStarted = false;
  bool _connected = false;
// Bluetooth related variables
  late DiscoveredDevice _ubiqueDevice;
  final flutterReactiveBle = FlutterReactiveBle();
  late StreamSubscription<DiscoveredDevice> _scanStream;
  late QualifiedCharacteristic _rxCharacteristic;
// These are the UUIDs of your device
  final Uuid serviceUuid = Uuid.parse("4fafc201-1fb5-459e-8fcc-c5c9c331914b");
  final Uuid characteristicUuid =
      Uuid.parse("beb5483e-36e1-4688-b7f5-ea07361b26a8");

  void _startScan() async {
// Platform permissions handling stuff
    bool permGranted = false;
    setState(() {
      _scanStarted = true;
      connectionText = "Scanning";
    });
    PermissionStatus permission;
    if (Platform.isAndroid) {
      permission = await LocationPermissions().requestPermissions();
      if (permission == PermissionStatus.granted) permGranted = true;
    } else if (Platform.isIOS) {
      permGranted = true;
    }
// Main scanning logic happens here ⤵️
    if (permGranted) {
      print('BLE: Permission Granted...');
      _scanStream = flutterReactiveBle
          .scanForDevices(withServices: [serviceUuid]).listen((device) {
        // Change this string to what you defined in Zephyr
        if (device.name == 'ESP32') {
          setState(() {
            _ubiqueDevice = device;
            _foundDeviceWaitingToConnect = true;
          });
        }
      });
    }
  }

  void _connectToDevice() {
    // We're done scanning, we can cancel it
    print('BLE: Connect to Device....');
    connectionText = "Connected";
    _scanStream.cancel();
    // Let's listen to our connection so we can make updates on a state change
    Stream<ConnectionStateUpdate> _currentConnectionStream = flutterReactiveBle
        .connectToAdvertisingDevice(
            id: _ubiqueDevice.id,
            prescanDuration: const Duration(seconds: 1),
            withServices: [serviceUuid, characteristicUuid]);
    _currentConnectionStream.listen((event) {
      switch (event.connectionState) {
        // We're connected and good to go!
        case DeviceConnectionState.connected:
          {
            print('BLE: Device connected.....');
            _rxCharacteristic = QualifiedCharacteristic(
                serviceId: serviceUuid,
                characteristicId: characteristicUuid,
                deviceId: event.deviceId);
            setState(() {
              _foundDeviceWaitingToConnect = false;
              _connected = true;
            });
            break;
          }
        // Can add various state state updates on disconnect
        case DeviceConnectionState.disconnected:
          {
            print('BLE: Device disconnected..');
            connectionText = "Disconnected";
            break;
          }
        default:
      }
    });
  }

  bool colorChangeUpdated = false; // Flag to track color changes
  void updateRGBviaBLE() {
    if (_connected) {
      connectionText = "Sent";
      alpha = currentColor.alpha;
      red = currentColor.red;
      blue = currentColor.blue;
      green = currentColor.green;
      // Assuming effect, alpha, red, blue, and green are int variables
      ByteData data = ByteData(5); // 4 bytes per int, 5 ints
      data.setInt8(0, effect);
      data.setInt8(1, alpha);
      data.setInt8(2, red);
      data.setInt8(3, green);
      data.setInt8(4, blue);

      flutterReactiveBle.writeCharacteristicWithResponse(_rxCharacteristic,
          value: data.buffer.asUint8List());
    }
    colorChangeUpdated = true;
    Future.delayed(const Duration(milliseconds: 75), () {
      // Reset the connectionText or perform additional actions if needed
      setState(() {
        colorChangeUpdated = false;
      });
    });
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text(connectionText),
      ),
      backgroundColor: currentColor,
      body: Center(
        child: ElevatedButton(
          onPressed: () => showPicker(),
          child: const Text(
            "Color",
            style: TextStyle(color: Colors.grey),
          ),
        ),
      ),
      persistentFooterButtons: [
        // We want to enable this button if the scan has NOT started
        // If the scan HAS started, it should be disabled.
        _scanStarted
            // True condition
            ? ElevatedButton(
                style: ElevatedButton.styleFrom(
                  primary: Colors.grey, // background
                  onPrimary: Colors.white, // foreground
                ),
                onPressed: () {},
                child: const Icon(Icons.search),
              )
            // False condition
            : ElevatedButton(
                style: ElevatedButton.styleFrom(
                  primary: Colors.blue, // background
                  onPrimary: Colors.white, // foreground
                ),
                onPressed: _startScan,
                child: const Icon(Icons.search),
              ),
        _foundDeviceWaitingToConnect
            // True condition
            ? ElevatedButton(
                style: ElevatedButton.styleFrom(
                  primary: Colors.blue, // background
                  onPrimary: Colors.white, // foreground
                ),
                onPressed: _connectToDevice,
                child: const Icon(Icons.bluetooth),
              )
            // False condition
            : ElevatedButton(
                style: ElevatedButton.styleFrom(
                  primary: Colors.grey, // background
                  onPrimary: Colors.white, // foreground
                ),
                onPressed: () {},
                child: const Icon(Icons.bluetooth_connected),
              ),
        _connected
            // True condition
            ? ElevatedButton(
                style: ElevatedButton.styleFrom(
                  primary: Colors.blue, // background
                  onPrimary: Colors.white, // foreground
                ),
                onPressed: updateRGBviaBLE,
                child: const Icon(Icons.send),
              )
            // False condition
            : ElevatedButton(
                style: ElevatedButton.styleFrom(
                  primary: Colors.grey, // background
                  onPrimary: Colors.white, // foreground
                ),
                onPressed: () {},
                child: const Icon(Icons.send),
              ),
      ],
    );
  }

  // create some values
  Color pickerColor = Color(0xff443a49);
  Color currentColor = Color(0xff443a49);

// ValueChanged<Color> callback
  void changeColor(Color color) {
    setState(() {
      pickerColor = color;
      currentColor = color;
      effect = 0;
      // Set the flag to true when color changes
      if (!colorChangeUpdated) updateRGBviaBLE();
    });
  }

  Future showPicker() {
    // raise the [showDialog] widget
    return showDialog(
      builder: (context) => AlertDialog(
        // title: const Text('Pick a color!'),
        content: SingleChildScrollView(
          child: ColorPicker(
            pickerColor: pickerColor,
            onColorChanged: changeColor,
            enableAlpha: true,
            labelTypes: [],
            paletteType: PaletteType.hueWheel,
            hexInputBar: false,
          ),
        ),
        actions: <Widget>[
          ElevatedButton(
            child: const Text('Got it'),
            onPressed: () {
              setState(() => currentColor = pickerColor);
              updateRGBviaBLE();
              Navigator.of(context).pop();
              print('Alpha: $alpha, Red:$red, Green:$green, Blue:$blue');
            },
          ),
          ElevatedButton(
            child: const Text('Rainbow'),
            onPressed: () {
              effect = 1;
              updateRGBviaBLE();
              Navigator.of(context)
                  .pop(); // Close the dialog without updating RGB
            },
          ),
          ElevatedButton(
            child: const Text('Confetti'),
            onPressed: () {
              effect = 3;
              updateRGBviaBLE();
              Navigator.of(context)
                  .pop(); // Close the dialog without updating RGB
            },
          ),
          ElevatedButton(
            child: const Text('Juggle'),
            onPressed: () {
              effect = 5;
              updateRGBviaBLE();
              Navigator.of(context)
                  .pop(); // Close the dialog without updating RGB
            },
          ),
        ],
      ),
      context: context,
    );
  }
}
