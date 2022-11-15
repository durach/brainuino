#define TIMER_START_60 60000 // ms
#define TIMER_START_20 20000 // ms

#define TIMER_PRECISION 1000 // ms

class Timer {
  private:
    uint32_t _startTime;
    uint32_t _oldValue;
    uint32_t _target;

  public:
    Timer();

    uint32_t value;
    bool isRunning = false;
    bool isUpdated = false;
    bool isFinished = false;
    
    void start(uint32_t target);
    void stop(bool updated = true);
    void tick();
    void reset();
};
