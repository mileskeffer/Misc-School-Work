class Configuration {
public:
    static Configuration& instance() {
        static Configuration object;
        return object;
    }

    Configuration(const Configuration&) = delete;
    Configuration& operator=(const Configuration&) = delete;

    Configuration(Configuration&&) = delete;
    Configuration& operator=(Configuration&&) = delete;

    void setVolume(int value) {
        volume_ = value;
    }

    int getVolume() const {
        return volume_;
    }

private:
    Configuration() = default;
    ~Configuration() = default;

    int volume_ = 50;
};