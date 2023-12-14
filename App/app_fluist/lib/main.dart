import 'dart:async';
import 'dart:io' show Platform;
import 'package:flutter/services.dart';

import 'package:location_permissions/location_permissions.dart';
import 'package:flutter/material.dart';
import 'package:flutter_reactive_ble/flutter_reactive_ble.dart';
import 'package:flutter_colorpicker/flutter_colorpicker.dart';

void main() async {
  WidgetsFlutterBinding.ensureInitialized();
  await SystemChrome.setPreferredOrientations([
    DeviceOrientation.portraitUp,
  ]).then((_) {
    runApp(const MaterialApp(
      debugShowCheckedModeBanner: false,
      home: HomePage(),
    ));
  });
}

class HomePage extends StatefulWidget {
  const HomePage({Key? key}) : super(key: key);

  @override
  _HomePageState createState() => _HomePageState();
}

class _HomePageState extends State<HomePage> {
  @override
  void initState() {
    super.initState();
    print("Scanning for FLUIST and immediately connecting if found...");
    _startScan();
  }

  @override
  void dispose() {
    super.dispose();
    _scanStream.cancel();
    print("Exiting...");
  }

// App Header String
  String connectionText = "Disconnected";
  String windowText = "Off";
  List<DiscoveredDevice> devices = [];

// ARGB
  double brightness = 0;
  int alpha = 0;
  int red = 0;
  int blue = 0;
  int green = 0;
  int effect = 0;
// Some state management stuff
  bool _scanStarted = false;
  bool _foundDeviceWaitingToConnect = false;
  bool _connected = false;
  bool _errorHandle = false;
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
      _errorHandle = true;
      connectionText = "Scanning";
    });
    PermissionStatus permission;
    if (Platform.isAndroid) {
      permission = await LocationPermissions().requestPermissions();
      if (permission == PermissionStatus.granted) permGranted = true;
    } else if (Platform.isIOS) {
      permGranted = true;
    }
// Main scanning logic happens here
    if (permGranted) {
      print('BLE: Permission Granted...');
      // Scan for all devices
      _scanStream =
          flutterReactiveBle.scanForDevices(withServices: []).listen((device) {
        if (!devices.contains(device)) {
          devices.add(device);
          print('Found device: ${device.name}, ID: ${device.id}');
          if (device.name == 'FLUIST') {
            // We're done scanning, we can cancel it
            _scanStream.cancel();
            setState(() {
              _ubiqueDevice = device;
              _foundDeviceWaitingToConnect = true;
              connectionText = "Found";
            });
            Future.delayed(const Duration(seconds: 1), () {
              // Reset the connectionText or perform additional actions if needed
              setState(() {
                _connectToDevice();
              });
            });
          }
        }
      });
    }
  }

  void _connectToDevice() {
    if (!_connected) {
      // We're done scanning, we can cancel it
      print('BLE: Connect to Device....');
      // Let's listen to our connection so we can make updates on a state change
      Stream<ConnectionStateUpdate> _currentConnectionStream =
          flutterReactiveBle.connectToAdvertisingDevice(
              id: _ubiqueDevice.id,
              prescanDuration: const Duration(seconds: 5),
              withServices: [serviceUuid, characteristicUuid]);
      print('BLE: Connection achived....');

      _currentConnectionStream.listen((event) {
        switch (event.connectionState) {
          // We're connected and good to go!
          case DeviceConnectionState.connected:
            {
              connectionText = "Connected";
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
              _connected = false;
              _scanStarted = false;
              Future.delayed(const Duration(seconds: 1), () {
                // Reset the connectionText or perform additional actions if needed
                setState(() {
                  _startScan();
                });
              });
              break;
            }
          default:
            {
              print('BLE: Processing....');
              break;
            }
        }
      });
    } else {
      print('BLE: Already connected....');
    }
  }

  void _errorHandling() {
    flutterReactiveBle.clearGattCache(_rxCharacteristic.deviceId);
    setState(() {
      _foundDeviceWaitingToConnect = false;
      _scanStarted = false;
      _connected = false;
      _errorHandle = false;
      effect = 0;
      currentColor = Color.fromARGB(255, 107, 108, 109);
      onoffColor = const Color.fromARGB(255, 52, 55, 63);
    });
    _startScan();
  }

  bool colorChangeUpdated = false; // Flag to track color changes
  void updateRGBviaBLE() {
    if (_connected) {
      // connectionText = "Sent";
      if (effect == 1) {
        alpha = currentColor.alpha;
      } else {
        alpha = (brightness * 255).toInt();
      }
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

  void powerOnOff() {
    setState(() {
      // ON
      if (_foundDeviceWaitingToConnect || _connected) {
        if (_connected) {
          if (effect == 0) {
            effect = 1;
            onoffColor = Colors.white;
            currentColor = Color.fromARGB(255, 226, 200, 122);
            windowText = "On";
            updateRGBviaBLE();
          } else {
            // OFF
            effect = 0;
            updateRGBviaBLE();
            currentColor = Color.fromARGB(255, 107, 108, 109);
            onoffColor = const Color.fromARGB(255, 52, 55, 63);
            windowText = "Off";
          }
        } else {
          _connectToDevice();
        }
      } else {
        if (!_scanStarted) _startScan();
      }
    });
  }

  String getImageAssetPathFromEffectValue(int effect) {
    // Implement logic to map BLE value to image asset path
    final Map<int, String> effectImagePaths = {
      0: 'assets/outline.png',
      1: 'assets/outline.png',
      2: 'assets/outline.png',
      3: 'assets/rainbow.png',
      4: 'assets/rainbow_glitter.png',
      5: 'assets/confetti.png',
      6: 'assets/outline.png',
      7: 'assets/juggle.png',
    };

    return effectImagePaths[effect] ?? 'assets/outline.png';
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text('Stained Fluist'),
        centerTitle: true,
        leading: Icon(Icons.light),
        backgroundColor: Color.fromARGB(255, 70, 74, 85),
      ),
      floatingActionButton: Column(
        mainAxisAlignment: MainAxisAlignment.end,
        children: [
          FloatingActionButton(
            backgroundColor: Color.fromARGB(255, 70, 74, 85),
            onPressed: () => showPicker(),
            child: Icon(
              Icons.color_lens,
              color: onoffColor,
            ),
          ),
          SizedBox(height: 16), // Add spacing between buttons
          FloatingActionButton(
            backgroundColor: Color.fromARGB(255, 70, 74, 85),
            onPressed: () => showEffects(),
            child: Icon(
              Icons.slideshow,
              color: onoffColor,
            ),
          ),
          SizedBox(height: 300), // Add spacing between buttons
          FloatingActionButton(
            backgroundColor: Color.fromARGB(255, 70, 74, 85),
            onPressed: () => powerOnOff(),
            child: Icon(
              Icons.power_settings_new,
              color: onoffColor,
            ),
          ),
        ],
      ),
      backgroundColor: Color.fromARGB(255, 52, 55, 63),
      body: Column(
        mainAxisAlignment: MainAxisAlignment.start,
        crossAxisAlignment: CrossAxisAlignment.center,
        children: [
          SizedBox(height: 16), // Add spacing
          RichText(
            text: TextSpan(
              text: 'BLE STATUS: ', // First part of the text with one color
              style: TextStyle(color: Color.fromARGB(255, 70, 74, 85)),
              children: <TextSpan>[
                TextSpan(
                  text: connectionText
                      .toUpperCase(), // Second part of the text with a different color
                  style: TextStyle(
                    color: Colors.white,
                    fontWeight: FontWeight.bold,
                  ),
                ),
              ],
            ),
          ),
          RichText(
            text: TextSpan(
              text: 'WINDOW STATUS: ', // First part of the text with one color
              style: TextStyle(color: Color.fromARGB(255, 70, 74, 85)),
              children: <TextSpan>[
                TextSpan(
                  text: windowText
                      .toUpperCase(), // Second part of the text with a different color
                  style: TextStyle(
                    color: Colors.white,
                    fontWeight: FontWeight.bold,
                  ),
                ),
              ],
            ),
          ),
          SizedBox(height: 100), // Add spacing
          Container(
            width: 300,
            color: currentColor,
            child: Image.asset(
              getImageAssetPathFromEffectValue(effect), // 'assets/outline.png'
            ),
          ),
          SizedBox(height: 20),
          Padding(
            padding: const EdgeInsets.symmetric(horizontal: 60),
            child: Column(
              children: [
                Row(
                  mainAxisAlignment: MainAxisAlignment.spaceBetween,
                  children: [
                    Container(
                      // Replace with your first icon or widget
                      child: Icon(
                        Icons.light_mode_outlined,
                        color: Colors.grey,
                      ),
                    ),
                    Expanded(
                      child: Slider(
                        value: brightness,
                        activeColor: Colors.grey,
                        inactiveColor: Color.fromARGB(255, 70, 74, 85),
                        thumbColor: Colors.white,
                        onChanged: (value) {
                          brightness = value;
                          changeBrightness(value);
                        },
                      ),
                    ),
                    Container(
                      // Replace with your second icon or widget
                      child: Icon(
                        Icons.light_mode,
                        color: Colors.grey,
                      ),
                    ),
                  ],
                ),
              ],
            ),
          ),
        ],
      ),
      //  persistentFooterButtons: [
      //    // We want to enable this button if the scan has NOT started
      //    // If the scan HAS started, it should be disabled.
      //    _scanStarted
      //        // True condition
      //        ? ElevatedButton(
      //            style: ElevatedButton.styleFrom(
      //              primary: Colors.grey, // background
      //              onPrimary: Colors.white, // foreground
      //            ),
      //            onPressed: () {},
      //            child: const Icon(Icons.search),
      //          )
      //        // False condition
      //        : ElevatedButton(
      //            style: ElevatedButton.styleFrom(
      //              primary: Colors.blue, // background
      //              onPrimary: Colors.white, // foreground
      //            ),
      //            onPressed: _startScan,
      //            child: const Icon(Icons.search),
      //          ),
      //    _foundDeviceWaitingToConnect
      //        // True condition
      //        ? ElevatedButton(
      //            style: ElevatedButton.styleFrom(
      //              primary: Colors.blue, // background
      //              onPrimary: Colors.white, // foreground
      //            ),
      //            onPressed: _connectToDevice,
      //            child: const Icon(Icons.bluetooth),
      //          )
      //        // False condition
      //        : ElevatedButton(
      //            style: ElevatedButton.styleFrom(
      //              primary: Colors.grey, // background
      //              onPrimary: Colors.white, // foreground
      //            ),
      //            onPressed: () {},
      //            child: const Icon(Icons.bluetooth_connected),
      //          ),
      //    _errorHandle
      //        // True condition
      //        ? ElevatedButton(
      //            style: ElevatedButton.styleFrom(
      //              primary: Color.fromARGB(255, 35, 35, 35), // background
      //              onPrimary: Colors.white, // foreground
      //            ),
      //            onPressed: _startScan,
      //            child: const Icon(Icons.bluetooth_disabled),
      //          )
      //        // False condition
      //        : ElevatedButton(
      //            style: ElevatedButton.styleFrom(
      //              primary: Colors.grey, // background
      //              onPrimary: Colors.white, // foreground
      //            ),
      //            onPressed: () {},
      //            child: const Icon(Icons.bluetooth_disabled),
      //          ),
      //  ],
    );
  }

  // create some values
  Color pickerColor = Color.fromARGB(255, 107, 108, 109);
  Color currentColor = Color.fromARGB(255, 107, 108, 109);
  Color effectColor1 = Color.fromARGB(255, 107, 108, 109);
  Color effectColor2 = Color.fromARGB(255, 107, 108, 109);
  Color onoffColor = const Color.fromARGB(255, 52, 55, 63);

// ValueChanged<Color> callback
  void changeColor(Color color) {
    setState(() {
      pickerColor = color;
      currentColor = color;
      effectColor1 = color;
      effectColor2 = color;
      effect = 1;
      // Set the flag to true when color changes
      if (!colorChangeUpdated) updateRGBviaBLE();
    });
  }

  void changeBrightness(double value) {
    setState(() {
      alpha = value.toInt();
      if (!colorChangeUpdated) updateRGBviaBLE();
    });
  }

  Future showPicker() {
    // raise the [showDialog] widget
    return showDialog(
      builder: (context) => AlertDialog(
        // title: const Text('Pick a color!'),
        backgroundColor: Color.fromARGB(255, 52, 55, 63),
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
              windowText = "Solid";
              setState(() => currentColor = pickerColor);
              effect = 1;
              updateRGBviaBLE();
              print('Alpha: $alpha, Red:$red, Green:$green, Blue:$blue');
              Navigator.of(context).pop();
            },
          ),
        ],
      ),
      context: context,
    );
  }

  Future showEffects() {
    // raise the [showDialog] widget
    return showDialog(
      builder: (context) => AlertDialog(
        title: const Text(
          'Effekte:',
          style: TextStyle(color: Colors.white),
        ),
        backgroundColor: Color.fromARGB(255, 52, 55, 63),
        actions: <Widget>[
          ElevatedButton(
            child: const Text('Fade'),
            onPressed: () {
              windowText = "Fade";
              effect = 2; // 2
              updateRGBviaBLE();
              Navigator.of(context)
                  .pop(); // Close the dialog without updating RGB
            },
          ),
          ElevatedButton(
            child: const Text('Rainbow'),
            onPressed: () {
              windowText = "Rainbow";
              effect = 3; //3
              updateRGBviaBLE();
              Navigator.of(context)
                  .pop(); // Close the dialog without updating RGB
            },
          ),
          ElevatedButton(
            child: const Text('Glitterbow'),
            onPressed: () {
              effect = 4; //4
              windowText = "Rainbow Glitter";
              updateRGBviaBLE();
              Navigator.of(context)
                  .pop(); // Close the dialog without updating RGB
            },
          ),
          ElevatedButton(
            child: const Text('Confetti'),
            onPressed: () {
              windowText = "Confetti";
              effect = 5; //5
              updateRGBviaBLE();
              Navigator.of(context)
                  .pop(); // Close the dialog without updating RGB
            },
          ),
          ElevatedButton(
            child: const Text('Juggle'),
            onPressed: () {
              windowText = "Juggle";
              effect = 6; //6
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
