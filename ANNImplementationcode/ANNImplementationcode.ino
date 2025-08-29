void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // Follow instructions and enter the values of inputs on serial monitor
  Serial.println("Please Enter pH(t-1) and pH(t-2)");
  while (Serial.available() == 0) {}
  float y1 = Serial.parseFloat(), y3 = Serial.parseFloat();
  while (Serial.available() == 0) {}
  String myNam = Serial.readString();

  Serial.println("Please Enter EC(t-1) and EC(t-2)");
  while (Serial.available() == 0) {}
  float y2 = Serial.parseFloat(), y4 = Serial.parseFloat();
  while (Serial.available() == 0) {}
  myNam = Serial.readString();

  Serial.println("Please Enter Temperature, T(t-1) and T(t-2)");
  while (Serial.available() == 0) {}
  float x1 = Serial.parseFloat(), x11 = Serial.parseFloat();
  while (Serial.available() == 0) {}
  myNam = Serial.readString();

  Serial.println("Please Enter Humidity, H(t-1) and H(t-2)");
  while (Serial.available() == 0) {}
  float x2 = Serial.parseFloat(), x21 = Serial.parseFloat();
  while (Serial.available() == 0) {}
  myNam = Serial.readString();

  Serial.println("Please Enter Water temperature, W_T(t-1) and W_T(t-2)");
  while (Serial.available() == 0) {}
  float x3 = Serial.parseFloat(), x31 = Serial.parseFloat();
  while (Serial.available() == 0) {}
  myNam = Serial.readString();

  Serial.println("Please Enter flow meter, F_M(t-1) and F_M(t-2)");
  while (Serial.available() == 0) {}
  float x4 = Serial.parseFloat(), x41 = Serial.parseFloat();
  while (Serial.available() == 0) {}
  myNam = Serial.readString();

  // Input 1
  float x11_offset = 35.8, x12_offset = 16, x13_offset = 17.31, x14_offset = 0;
  float x11_gain = 0.0300751879699248, x12_gain = 0.109289617486339,
        x13_gain = 0.126984126984127, x14_gain = 0.0289855072463768;
  float x1_ymin = -1;

  // Input 2
  float x21_offset = 4.89, x22_offset = 2.21;
  float x21_gain = 1.04712041884817, x22_gain = 2.40963855421687;
  float x2_ymin = -1;

  // Output 1
  float y1_ymin = -1;
  float y21_gain = 1.04712041884817, y22_gain = 2.40963855421687;
  float y21_offset = 4.89, y22_offset = 2.21;

  // Formatting Inputs
  float X1 = ((x1 - x11_offset) * x11_gain) + x1_ymin;
  float X2 = ((x2 - x12_offset) * x12_gain) + x1_ymin;
  float X3 = ((x3 - x13_offset) * x13_gain) + x1_ymin;
  float X4 = ((x4 - x14_offset) * x14_gain) + x1_ymin;
  float X5 = ((x11 - x11_offset) * x11_gain) + x1_ymin;
  float X6 = ((x21 - x12_offset) * x12_gain) + x1_ymin;
  float X7 = ((x31 - x13_offset) * x13_gain) + x1_ymin;
  float X8 = ((x41 - x14_offset) * x14_gain) + x1_ymin;

  float Y1 = ((y1 - x21_offset) * x21_gain) + x2_ymin;
  float Y2 = ((y2 - x22_offset) * x22_gain) + x2_ymin;
  float Y3 = ((y3 - x21_offset) * x21_gain) + x2_ymin;
  float Y4 = ((y4 - x22_offset) * x22_gain) + x2_ymin;

  // Layer 1
  float b11 = 0.0473704809714359, b12 = -0.5806516565898644;
  float IW1_0 = -0.0008856522227611, IW1_1 = 0.0161774754418859, 
        IW1_2 = 0.1285782840628543, IW1_3 = 0.0168350110352804,
        IW1_4 = 0.00100824940364, IW1_5 = -0.0073590030438688,
        IW1_6 = -0.1172341383019037, IW1_7 = -0.00536501198350635;

  float IW2_0 = -0.0393852192867964, IW2_1 = -0.008094114470252,
        IW2_2 = 0.0898034617348214, IW2_3 = 0.0101587711033461,
        IW2_4 = -0.00268831439242449, IW2_5 = -0.0313776554579349,
        IW2_6 = -0.0954144351725477, IW2_7 = -0.0954144351725477;

  float IW3_0 = -0.049684478808088, IW3_1 = 0.305382559627246,
        IW3_2 = -0.0236362311727722, IW3_3 = 0.0654834644940186,
        IW3_4 = 0.766194293652999, IW3_5 = 0.199912975228452,
        IW3_6 = 0.579164334112892, IW3_7 = 0.00623338232344978;

  // Layer 2
  float b21 = 0.437993098993609, b22 = -0.00458017389654517;
  float LW21_0 = -0.361632360094489, LW22_0 = 0.695118370340954,
        LW21_1 = 2.48403931053471, LW22_1 = 0.140365355794936;

  // Outputs Layer 1
  float Z11 = ((IW1_0*X1) + (IW1_1*X2) + (IW1_2*X3) + (IW1_3*X4)
              + (IW1_4*X5) + (IW1_5*X6) + (IW1_6*X7) + (IW1_7*X8));

  float Z12 = ((IW3_0*Y1) + (IW3_1*Y2) + (IW3_2*Y3) + (IW3_3*Y4));

  float Z21 = ((IW2_0*X1) + (IW2_1*X2) + (IW2_2*X3) + (IW2_3*X4)
              + (IW2_4*X5) + (IW2_5*X6) + (IW2_6*X7) + (IW2_7*X8));

  float Z22 = ((IW3_4*Y1) + (IW3_5*Y2) + (IW3_6*Y3) + (IW3_7*Y4));

  float Z1 = Z11 + Z12 + b11;
  float Z2 = Z21 + Z22 + b12;

  float fZ1 = 2 / (1 + exp(-2 * Z1)) - 1;
  float fZ2 = 2 / (1 + exp(-2 * Z2)) - 1;

  // Layer 2 outputs
  float Z3 = (LW21_0 * fZ1) + (LW22_0 * fZ2) + b21;
  float Z4 = (LW21_1 * fZ1) + (LW22_1 * fZ2) + b22;

  // Normalizing Output
  float fZ3 = ((Z3 - y1_ymin) / y21_gain) + y21_offset - 0.06;
  float fZ4 = ((Z4 - y1_ymin) / y22_gain) + y22_offset;

  // Display results
  Serial.print("pH predicted is: ");
  Serial.println(fZ3);
  Serial.print("EC predicted is: ");
  Serial.println(fZ4);

  delay(10000);
  Serial.println();
}

