using System;
using System.IO;
using System.Globalization;
using System.Linq;
using System.Collections.Generic;
using System.Diagnostics;

using static Generator;
using CustomExtensions;
using CustomUtilities;
using static CustomUtilities.Headers;
using static CustomUtilities.If;

static class Tester {
  public static string tests_path = "./test_cases";
  public static string tests_txt_path = $"{tests_path}/txt";
  static void Main() {
    // SimpleTest.normalization();
    Directory.CreateDirectory(tests_txt_path);
    ConversionSample.print();

    /* arithmetic operations */
    Tester.CommutativeBinaryOpResults("add");
    Tester.CommutativeBinaryOpResults("mul");
    Tester.NCommutativeBinaryOpResults("sub");
    Tester.NCommutativeBinaryOpResults("div");
    Tester.NCommutativeBinaryOpResults("mod");
    /* comparison operations */
    Tester.ComparisonOpResults("is_less");
    Tester.ComparisonOpResults("is_less_or_equal");
    Tester.ComparisonOpResults("is_greater");
    Tester.ComparisonOpResults("is_greater_or_equal");
    Tester.ComparisonOpResults("is_equal");
    Tester.ComparisonOpResults("is_not_equal");
    // /* convertors and parsers */
    {
      unary_op_cases_n = 200;
      decimal_cases = Generator.GenerateDecimalCases();
      int_cases = Generator.GenerateIntCases();
      float_cases = Generator.GenerateFloatCases();
    }
    Tester.ConvertOpResults("from_int_to_decimal");
    Tester.ConvertOpResults("from_float_to_decimal");
    Tester.ConvertOpResults("from_decimal_to_int");
    Tester.ConvertOpResults("from_decimal_to_float");
    /* other functions */
    Tester.UnaryOpResults("negate");
    Tester.UnaryOpResults("truncate");
    Tester.UnaryOpResults("round");
    Tester.UnaryOpResults("floor");
  }
  public static List<decimal> cases = new List<decimal>() {
    0m,
    -0.1m,
    0.1m,
    -1m,
    1m,
    -1.0m,
    1.0m,
    -1m / 3m,
    1m / 3m,
    -1m / 3m * 3m,
    1m / 3m * 3m,
    -5m / 3m,
    5m / 3m,
    -5m / 11m,
    5m / 11m,
    -15m / 13m,
    15m / 13m,
    -1.0000000000000000000000000000m,
    1.0000000000000000000000000000m,
    -0.0000000000000000000000000001M,
    0.0000000000000000000000000001M,
    -7.9228162514264337593543950335M,
    +7.9228162514264337593543950335M,
    decimal.MinValue,
    decimal.MaxValue,
  };
  public static int unary_op_cases_n;
  public static int cases_n = cases.Count;
  public static List<Decimal> decimal_cases;
  public static List<Single> float_cases;
  public static List<Int32> int_cases;
  static void CommutativeBinaryOpResults(string op) {
    string path = $"{tests_txt_path}/arithmetics/";
    Directory.CreateDirectory(path);
    var bw = new BinaryWriter(File.Open($"{tests_path}/{op}.bin", FileMode.Create));
    var dw = new StreamWriter(File.Open($"{path}/{op}.txt", FileMode.Create));
    Headers.WriteBinaryOpHeader(dw);
    decimal a, b, res = 0, res_norm = 0;
    string sres, symbol = "?";
    Int32 ret;
    int n = 0;

    for (int i = 0; i < cases_n; ++i) {
      for (int j = i; j < cases_n; ++j, n++) {
        a = cases[i];
        b = cases[j];

        try {
          switch (op) {
            case "add":
              symbol = "+";
              res = a + b;
              break;
            case "mul":
              symbol = "*";
              res = a * b;
              break;
            default:
              Terminate.PrintError(Terminate.GetCurrentMethod(), op);
              break;
          }
          ret = 0;
          sres = res.ToString();
          res_norm = res.NormalizeBits();
        } catch {
          if ((op == "mul" && If.SignsAreEqual(a, b)) ||
              (op == "add" && If.SignsAreEqual(a, b) && Math.Sign(a) == 1)) {
            ret = 1;
            sres = "+inf";
          } else {
            ret = 2;
            sres = "-inf";
          }
        }
        bw.Serialize(a, b, ret, res_norm);
        var hex_norm = ret == 0 ? res_norm.GetHexString() : "undefined behaviour";
        var hex = ret == 0 ? res.GetHexString() : "undefined behaviour";
        dw.WriteLine(
            $"{n,3}|{a,32} {symbol} {b,31} = {sres,31} | {ret} | {hex_norm,-35} | {hex,-35} = {a.GetHexString()} {symbol} {b.GetHexString()}");
      }
    }
    bw.Close();
    dw.Close();
  }
  static void NCommutativeBinaryOpResults(string op) {
    string path = $"{tests_txt_path}/arithmetics/";
    Directory.CreateDirectory(path);
    var bw = new BinaryWriter(File.Open($"{tests_path}/{op}.bin", FileMode.Create));
    var dw = new StreamWriter(File.Open($"{path}/{op}.txt", FileMode.Create));
    Headers.WriteBinaryOpHeader(dw);
    decimal a, b, res = 0, res_norm = 0;
    string sres, symbol = "?";
    Int32 ret;
    int n = 0;

    for (int i = 0; i < cases_n; ++i) {
      for (int j = 0; j < cases_n; ++j, ++n) {
        a = cases[i];
        b = cases[j];

        try {
          switch (op) {
            case "sub":
              symbol = "-";
              res = a - b;
              break;
            case "div":
              symbol = "/";
              res = a / b;
              break;
            case "mod":
              symbol = "%";
              res = a % b;
              break;
            default:
              Terminate.PrintError(Terminate.GetCurrentMethod(), op);
              break;
          }
          ret = 0;
          sres = res.ToString();
          res_norm = res.NormalizeBits();
        } catch (DivideByZeroException) {
          ret = 3;
          sres = "NaN";
        } catch {
          if (op == "sub" && !SignsAreEqual(a, b) && Math.Sign(b) == -1 ||  //
              op != "sub" && SignsAreEqual(a, b)) {
            ret = 1;
            sres = "+inf";
          } else {
            ret = 2;
            sres = "-inf";
          }
        }
        bw.Serialize(a, b, ret, res);
        var hex_norm = ret == 0 ? res_norm.GetHexString() : "undefined behaviour";
        var hex = ret == 0 ? res.GetHexString() : "undefined behaviour";
        dw.WriteLine(
            $"{n,3}|{a,32} {symbol} {b,31} = {sres,31} | {ret} | {hex_norm,-35} | {hex,-35} = {a.GetHexString()} {symbol} {b.GetHexString()}");
      }
    }
    bw.Close();
    dw.Close();
  }
  static void ComparisonOpResults(string op) {
    string path = $"{tests_txt_path}/comparisons/";
    Directory.CreateDirectory(path);
    var bw = new BinaryWriter(File.Open($"{tests_path}/{op}.bin", FileMode.Create));
    var dw = new StreamWriter(File.Open($"{path}/{op}.txt", FileMode.Create));
    Headers.WriteComparisonOpHeader(dw);
    decimal a, b;
    string symbol = "?";
    Int32 ret = 0;
    int n = 0;

    for (int i = 0; i < cases_n; ++i) {
      for (int j = 0; j < cases_n; ++j, ++n) {
        a = cases[i];
        b = cases[j];

        switch (op) {
          case "is_less":
            symbol = "<";
            ret = a < b ? 1 : 0;
            break;
          case "is_less_or_equal":
            symbol = "<=";
            ret = a <= b ? 1 : 0;
            break;
          case "is_greater":
            symbol = ">";
            ret = a > b ? 1 : 0;
            break;
          case "is_greater_or_equal":
            symbol = ">=";
            ret = a >= b ? 1 : 0;
            break;
          case "is_equal":
            symbol = "==";
            ret = a == b ? 1 : 0;
            break;
          case "is_not_equal":
            symbol = "!=";
            ret = a != b ? 1 : 0;
            break;
          default:
            Terminate.PrintError(Terminate.GetCurrentMethod(), op);
            break;
        }
        bw.Serialize(a, b, ret);
        dw.WriteLine(
            $"{n,3}|{a,31} {symbol,2} {b,31} | {ret} | {a.GetHexString()} | {b.GetHexString()}");
      }
    }
    bw.Close();
    dw.Close();
  }
  static void UnaryOpResults(string op) {
    string path = $"{tests_txt_path}/others/";
    Directory.CreateDirectory(path);
    var bw = new BinaryWriter(File.Open($"{tests_path}/{op}.bin", FileMode.Create));
    var dw = new StreamWriter(File.Open($"{path}/{op}.txt", FileMode.Create));
    Headers.WriteOthersOpHeader(dw);
    decimal a, res = decimal.Zero;
    Int32 ret;
    int n = 0;

    for (int i = 0; i < unary_op_cases_n; ++i, ++n) {
      a = decimal_cases[i];
      switch (op) {
        case "negate":
          res = decimal.Negate(a);
          break;
        case "truncate":
          res = decimal.Truncate(a);
          break;
        case "round":
          res = decimal.Round(a);
          break;
        case "floor":
          res = decimal.Floor(a);
          break;
        default:
          Terminate.PrintError(Terminate.GetCurrentMethod(), op);
          break;
      }
      ret = 0;
      bw.Serialize(a, ret, res);
      dw.WriteLine($"{n,3}|{a,32}|{res,32}|{ret,2} | {a.GetHexString()} -> {res.GetHexString()}");
    }
    bw.Close();
    dw.Close();
  }
  static void ConvertOpResults(string op) {
    string path = $"{tests_txt_path}/convertors/";
    Directory.CreateDirectory(path);
    var bw = new BinaryWriter(File.Open($"{tests_path}/{op}.bin", FileMode.Create));
    var dw = new StreamWriter(File.Open($"{path}/{op}.txt", FileMode.Create));
    Headers.WriteConvertorOpHeader(dw);
    Int32 in_int, res_int = 0;
    Single in_float, res_float = 0f;
    Decimal in_dec, res_dec = Decimal.Zero;
    String in_str = "placeholder", res_str = "placeholder";
    int n = 0, left_field = 0, right_field = 0;
    Int32 ret;

    for (int i = 0; i < unary_op_cases_n; ++i, ++n) {
      try {
        switch (op) {
          case "from_int_to_decimal":
            in_int = int_cases[i];
            in_str = Convert.ToString(in_int);
            res_dec = Convert.ToDecimal(in_int);
            res_str = Convert.ToString(res_dec);
            left_field = 12;
            right_field = 12;

            bw.Write(in_int);
            bw.Write(res_dec);
            break;
          case "from_float_to_decimal":
            in_float = float_cases[i];
            in_str = Convert.ToString(in_float);
            res_dec = Convert.ToDecimal(in_float);
            res_str = Convert.ToString(res_dec);
            left_field = 14;
            right_field = 14;

            bw.Write(in_float);
            bw.Write(res_dec);
            break;
          case "from_decimal_to_int":
            in_dec = decimal_cases[i];
            in_str = Convert.ToString(in_dec);
            res_int = Convert.ToInt32(in_dec);
            res_str = Convert.ToString(res_int);
            left_field = 32;
            right_field = 12;

            bw.Write(in_dec);
            bw.Write(res_int);
            break;
          case "from_decimal_to_float":
            in_dec = decimal_cases[i];
            in_str = Convert.ToString(in_dec);
            res_float = Convert.ToSingle(in_dec);
            res_str = Convert.ToString(res_float);
            left_field = 32;
            right_field = 14;

            bw.Write(in_dec);
            bw.Write(res_float);
            break;
          default:
            Terminate.PrintError(Terminate.GetCurrentMethod(), op);
            break;
        }
        ret = 0;
      } catch {
        ret = 1;
        res_str = "error";
      }
      // dw.WriteLine($"{n,3}|{in_str, 32}{res_str,30}|{ret,2}");

      // var fmt = $"{n, 3}|{{0, {in_str.Length}}} {op} {res_str,30}|{ret,2}";
      // dw.WriteLine($"{n,3}|{in_str,in_str.Length} {op} {res_str,30}|{ret,2}");
      // dw.WriteLine(fmt, in_str);

      // var fmt = $"{n, 3}|{{0, {left_field}}} {op} {{1, {right_field}}}|{ret,2}";
      // dw.WriteLine(fmt, in_str, res_str);
      var fmt = $"{n, 3}|{{0, {left_field}}}|{{1, {right_field}}}|{ret,2} | ";
      dw.Write(fmt, in_str, res_str);
      if (op == "from_int_to_decimal" || op == "from_float_to_decimal") {
        dw.WriteLine((ret == 0 ? res_dec.GetHexString() : "undefined behaviour"));
      } else if (op == "from_decimal_to_float") {
        // dw.WriteLine(Convert.ToInt32(res_float).ToString("X8"));
        // dw.WriteLine(BitConverter.GetBytes(res_float));
        // dw.WriteLine($"{BitConverter.GetBytes(res_float),8:X8}");
        dw.WriteLine(res_float.GetHexString());
      } else if (op == "from_decimal_to_int") {
        dw.WriteLine(res_int.GetHexString());
      }

      // dw.WriteLine(("{0,-11}|{1,2} {3} {4,5}|{6,2}", n, in_str, left_field, op, res_str,
      // right_field, ret));
    }
    bw.Close();
    dw.Close();
  }
}

public static class Generator {
  public static int NextInt32(this Random rng) {
    int firstBits = rng.Next(0, 1 << 4) << 28;
    int lastBits = rng.Next(0, 1 << 28);
    return firstBits | lastBits;
  }
  public static decimal NextDecimal(this Random rng) {
    byte scale = (byte)rng.Next(29);
    bool sign = rng.Next(2) == 1;
    return new decimal(rng.NextInt32(), rng.NextInt32(), rng.NextInt32(), sign, scale);
  }
  public static float NextFloat(Random random) {
    var buffer = new byte[4];
    random.NextBytes(buffer);
    return BitConverter.ToSingle(buffer, 0);
  }

  public static Random rnd = new Random();
  public static List<decimal> GenerateDecimalCases() {
    var decimal_cases = new List<decimal>(Tester.cases);
    for (int i = Tester.cases_n; i < Tester.unary_op_cases_n; ++i) {
      decimal_cases.Add(rnd.NextDecimal());
    }
    return decimal_cases;
  }
  public static List<Int32> GenerateIntCases() {
    var int_cases = new List<Int32>();
    int_cases.Add(0);
    int_cases.Add(1);
    int_cases.Add(-1);
    int_cases.Add(int.MinValue);
    int_cases.Add(int.MaxValue);
    for (int i = 0; i < Tester.unary_op_cases_n; ++i) {
      int_cases.Add(rnd.NextInt32());
    }
    return int_cases;
  }
  public static List<Single> GenerateFloatCases() {
    var float_cases = new List<Single>();
    float_cases.Add(0f);
    // float_cases.Add(Single.NegativeZero);
    float_cases.Add(0.1f);
    float_cases.Add(-0.1f);
    float_cases.Add(0.0001f);
    float_cases.Add(-0.0001f);
    float_cases.Add(1f);
    float_cases.Add(-1f);
    // float_cases.Add(float.E);
    float_cases.Add(Single.Epsilon);
    // float_cases.Add(Single.Tau);
    // float_cases.Add(Single.Nan);
    float_cases.Add(Single.NegativeInfinity);
    float_cases.Add(Single.PositiveInfinity);
    float_cases.Add(Single.MinValue);
    float_cases.Add(Single.MaxValue);
    for (int i = 0; i < Tester.unary_op_cases_n; ++i) {
      float_cases.Add(rnd.NextInt32());
    }
    return float_cases;
  }
}

namespace CustomExtensions {
  public static class DecimalExtensions {
    public static decimal Normalize(this decimal value) {
      return value / 1.000000000000000000000000000000000m;
    }
    public static decimal NormalizeBits(this decimal d) {
      return decimal.Parse(d.Normalize().ToString());
    }
  }
  public static class BinaryWriterExtensions {
    public static void Serialize(this BinaryWriter bw, decimal a, decimal b, Int32 ret,
                                 decimal res) {
      bw.Write(a);
      bw.Write(b);
      bw.Write(ret);
      bw.Write(res);
    }
    public static void Serialize(this BinaryWriter bw, decimal a, decimal b, Int32 ret) {
      bw.Write(a);
      bw.Write(b);
      bw.Write(ret);
    }
    public static void Serialize(this BinaryWriter bw, decimal a, Int32 ret) {
      bw.Write(a);
      bw.Write(ret);
    }
    public static void Serialize(this BinaryWriter bw, decimal a, Int32 ret, decimal res) {
      bw.Write(a);
      bw.Write(ret);
      bw.Write(res);
    }
    // public static void Serialize(this BinaryWriter bw, params object[] list) {
    //   for (int i = 0; i < list.Length; ++i) {
    //     bw.Write(list[i]);
    //   }
    // }
  }
  public static class StringExtensions {
    public static string Repeat(this char c, int n) {
      return new String(c, n);
    }
    public static string GetHexString(this decimal d) {
      var bits = decimal.GetBits(d);
      return $"{bits[3],8:X8}{bits[2],9:X8}{bits[1],9:X8}{bits[0],9:X8}";
    }
    public static string GetHexString(this int i) {
      return i.ToString("X8");
    }
    public static string GetHexString(this Single s) {
      var bytes = BitConverter.GetBytes(s);
      return BitConverter.ToInt32(bytes,0).GetHexString();
    }
    public static string GetBinaryString(this decimal d) {
      var bits = decimal.GetBits(d);
      var binary_string = string.Empty;

      for (int i = 3; i >= 0; --i) {
        if (i == 3) {
          binary_string += $"{Convert.ToString(bits[i], 2).PadLeft(32, '0')}";
          continue;
        }
        binary_string += $"{Convert.ToString(bits[i], 2).PadLeft(32, '0'), 33}";
      }

      return binary_string;
    }
  }
}

namespace CustomUtilities {
  public static class Terminate {
    public static string GetCurrentMethod() {
      var st = new StackTrace();
      var sf = st.GetFrame(1);

      return sf.GetMethod().Name;
    }
    public static void PrintError(string method_name, string op) {
      Console.WriteLine($"Incorrect function \"{method_name}\" input: \"{op}\"");
      System.Environment.Exit(1);
    }
  }
  public static class Headers {
    public static void WriteBinaryOpHeader(StreamWriter dw) {
      dw.WriteLine(
          $" n |{"a",16}{"|",18}{"b", 17}{"|",17}{"result", 19}{"|",15}{"ret", 3}|{"normalized result in hex",30}{"|",8}{"result in hex",25}{"|",13}{"a in hex",23}{"|",15}{"b in hex",23}");
      dw.WriteLine('-'.Repeat(260));
    }
    public static void WriteComparisonOpHeader(StreamWriter dw) {
      dw.WriteLine(
          $"{" n "}|{"a",16}{"|",16}op|{"b", 17}{"|",16}{"ret", 3}|{"a in hex",23}{"|",15}{"b in hex",23}");
      dw.WriteLine('-'.Repeat(150));
    }
    public static void WriteOthersOpHeader(StreamWriter dw) {
      dw.WriteLine(
          $"{" n "}|{"argument",20}{"|",13}{"result", 19}{"|",14}{"ret", 3}|{"argument in hex",26}{"result in hex",36}");
      dw.WriteLine('-'.Repeat(149));
    }
    public static void WriteConvertorOpHeader(StreamWriter dw) {
      // public static void WriteConvertorOpHeader(StreamWriter dw, string op) {
      // string fmt = $"{" n
      // "}|{"a",17}{"|",16}{{0,{(op.Length+8)/2}}}{{1,{(op.Length-8+1)/2}}}|{"result",
      // 8}{"|",4}{"ret", 3}"; dw.WriteLine(string.Format(fmt, "function", " "));
      dw.WriteLine(" n |");
      dw.WriteLine('-'.Repeat(74));
    }
  }
  public static class If {
    public static bool SignsAreEqual(decimal a, decimal b) {
      return Math.Sign(a) == Math.Sign(b);
    }
  }
}

public static class ConversionSample {
  public static void print() {
    string path = $"{Tester.tests_txt_path}/conversion_sample.txt";
    var sw = new StreamWriter(File.Open(path, FileMode.Create));

    Decimal[] values = { Decimal.Zero,
                         0.0000000000000000000000000000M,
                         1M,
                         -1M,
                         0.1M,
                         -0.1M,
                         1M / 9M,
                         -1M / 9M,
                         1M,
                         0.0000000000000000000000000001M,
                         -0.0000000000000000000000000001M,
                         5M / 11M,
                         -5M / 11M,
                         0.5M,
                         -0.5M,
                         1M,
                         2M,
                         3M,
                         1M,
                         1.0M,
                         -1.0M,
                         1.00M,
                         1.0000M,
                         1.0000000000000000000000000000M,
                         100000000000000M,
                         10000000000000000000000000000M,
                         100000000000000.00000000000000M,
                         15M / 13M,
                         -15M / 13M,
                         1M / 3M,
                         2M / 3M,
                         1M / 3M * 3M,
                         123456789M,
                         0.123456789M,
                         0.000000000123456789M,
                         0.000000000000000000123456789M,
                         4294967295M,
                         18446744073709551615M,
                         Decimal.MaxValue,
                         Decimal.MinValue,
                         Decimal.MinValue + 1,
                         -7.9228162514264337593543950335M,
                         +7.9228162514264337593543950335M };

    var f1 = '-'.Repeat(20);
    var f2 = '-'.Repeat(8);
    var f3 = '-'.Repeat(32);
    sw.WriteLine("{0,31}  {1,8} {2,8} {3,8} {4,8}  {1,-32} {2,-32} {3,-32} {4,-32}  {0,-32}",
                 "Decimal type number", "Bits[3]", "Bits[2]", "Bits[1]", "Bits[0]");
    sw.WriteLine(
        $"{f1,31}  {f2,8} {f2,8} {f2,8} {f2,8}  {f3,-32} {f3,-32} {f3,-32} {f3,-32}  {f1,-32}");
    foreach (var value in values) {
      sw.WriteLine($"{value,31}  {value.GetHexString()}  {value.GetBinaryString()}  {value,31}");
    }

    sw.WriteLine("{0}{1}", ' '.Repeat(70), "|      |<------><-------------->");
    sw.WriteLine("{0}{1}", ' '.Repeat(70), "|      [exponent]  [not used]");
    sw.WriteLine("{0}{1}", ' '.Repeat(66), "[sign bit]");
    sw.WriteLine("{0}{1}", ' '.Repeat(71), "|<---->|");
    sw.WriteLine("{0}{1}", ' '.Repeat(70), "[not used]");

    sw.Close();
  }
}

static class SimpleTest {
  public static void mod() {
    var cases = new Tuple<decimal, decimal>[] {
      Tuple.Create(7m, 3m),
      Tuple.Create(-7m, 3m),
      Tuple.Create(7m, -3m),
      Tuple.Create(-7m, -3m),
      Tuple.Create(18446744069414584320m, 0.00000000000000009m),
      Tuple.Create(-18446744069414584320m, 0.00000000000000009m),
    };
    decimal a, b;
    for (int i = 0; i < cases.Length; ++i) {
      a = cases[i].Item1;
      b = cases[i].Item2;
      Console.WriteLine($"test: {a} % {b} = {a % b}");
    }
  }
  public static void normalization() {
    decimal[] cases = { 0m, Decimal.Add(-1m, 1.0m), 0.1m, 0.10m, 0.100m, 0.1000000m };
    decimal d;

    for (int i = 0; i < cases.Length; ++i) {
      d = cases[i];
      Console.WriteLine($"{d,32}  {d.GetHexString()}  {d.GetBinaryString()}");
    }

    Console.WriteLine("Normalized output");
    for (int i = 0; i < cases.Length; ++i) {
      d = cases[i];
      Console.WriteLine($"{d.Normalize(),32}  {d.GetHexString()}  {d.GetBinaryString()}");
    }

    Console.WriteLine("Normalized bit strings");
    for (int i = 0; i < cases.Length; ++i) {
      d = cases[i].NormalizeBits();
      Console.WriteLine($"{d,32}  {d.GetHexString()}  {d.GetBinaryString()}");
    }
  }

  // public static void writer() {
  //   var bw = new BinaryWriter(File.Open($"check.bin", FileMode.Create));
  //   bw.Serialize(1m, 2m, 5);
  // }
}
