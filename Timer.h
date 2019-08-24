#define TIMER_START_60 60.0
#define TIMER_START_20 20.0
#define TIMER_FINISH 0

#define TIMER_MILLISECONDS_IN_SECOND 1000.0
#define TIMER_PERCISION 0.01

class Timer {
  private:
    double _start_time;
    double _old_value;
    unsigned long _target;

  public:
    Timer();

    double value;
    bool is_running;
    bool is_updated = false;
    
    void start(double target);
    void stop();
    void tick();
    void reset();
};

