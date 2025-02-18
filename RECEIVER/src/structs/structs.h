enum LightType
{
  STATIC = 0,
  CHASE = 1
};

struct CHANGE_MESSAGE
{
  // light
  LightType light__type; // 0: static, 1: chase
  int r;
  int g;
  int b;
  int w;  // not used
  int ww; // not used
  int light__transition;

  // motor
};