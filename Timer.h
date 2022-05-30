#define TIMER_START_60 60.0
#define TIMER_START_20 20.0

#define TIMER_MILLISECONDS_IN_SECOND 1000.0
#define TIMER_PRECISION 1.0

class Timer {
  private:
    unsigned long _startTime;
    double _oldValue;
    double _target;

  public:
    Timer();

    double value;
    bool isRunning = false;
    bool isUpdated = false;
    bool isFinished = false;
    
    void start(double target);
    void stop(bool updated = true);
    void tick();
    void reset();
};
