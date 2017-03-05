//See [C:/Demo/Games/Stadium/StadiumGen.HC.Z,1].

U0 Main()
{
  "Capture scrn...\n";
  PressAKey;
  GRScrnCaptureWrite("~/DemoScrnShot");
  "View captured scrn...\n";
  PressAKey;
  GRScrnCaptureRead("~/DemoScrnShot");
  PressAKey;
  DCFill;
}

Main;
