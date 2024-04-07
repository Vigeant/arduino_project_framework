

class Context; // Forward declaration of the Context class

class State {
public:
    virtual ~State() {}

    // Called when entering a state
    virtual void onEntry(Context& context){}

    // Called when exiting a state
    virtual void onExit(Context& context){}

    virtual void handle(Context& context) = 0;
};

class Context {
private:
    State* state;  // raw pointer instead of unique_ptr

public:
    Context() : state(nullptr) {}
    void setState(State* newState);  // accept raw pointer in setState
    void setInitialState(State* initialState) { state = initialState; }
    void request();
    State* getState() const { return state; }  // getter remains the same
};