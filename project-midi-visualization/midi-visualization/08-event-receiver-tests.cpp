#include "settings.h"

#ifdef TEST_BUILD

#include "midi.h"
#include "Catch.h"
#include <sstream>
#include <list>


namespace
{
    struct Event
    {
        uint32_t dt;

        virtual ~Event() { }
    };

    struct MetaEvent : public Event
    {
        uint8_t type;
        std::unique_ptr<char[]> data;
        int data_size;
    };

    struct SysexEvent : public Event
    {
        std::unique_ptr<char[]> data;
        int data_size;
    };

    struct MidiEvent : public Event
    {
        uint8_t channel;
    };

    struct NoteOnEvent : public MidiEvent
    {
        uint8_t note;
        uint8_t velocity;
    };

    struct NoteOffEvent : public MidiEvent
    {
        uint8_t note;
        uint8_t velocity;
    };

    struct PolyphonicEvent : public MidiEvent
    {
        uint8_t note;
        uint8_t pressure;
    };

    struct ControlChangeEvent : public MidiEvent
    {
        uint8_t controller;
        uint8_t value;
    };

    struct ProgramChangeEvent : public MidiEvent
    {
        uint8_t program;
    };

    struct ChannelPressureEvent: public MidiEvent
    {
        uint8_t pressure;
    };

    struct PitchWheelChangeEvent : public MidiEvent
    {
        uint16_t value;
    };

    class TestEventReceiver : public EventReceiver
    {
    private:
        std::list<std::unique_ptr<Event>> expected_events;

    public:
        TestEventReceiver(std::list<std::unique_ptr<Event>> expected_events) : expected_events(std::move(expected_events)) { }        

        void note_on(uint32_t dt, uint8_t channel, uint8_t note, uint8_t velocity) override
        {
            REQUIRE(expected_events.size() != 0);

            std::unique_ptr<Event> expected = std::move(expected_events.front());
            expected_events.pop_front();
            auto actual = dynamic_cast<NoteOnEvent*>(expected.get());

            REQUIRE(actual != nullptr);
            CHECK(actual->dt == dt);
            CHECK(actual->channel == channel);
            CHECK(actual->note == note);
            CHECK(actual->velocity == velocity);
        }

        void note_off(uint32_t dt, uint8_t channel, uint8_t note, uint8_t velocity) override
        {
            REQUIRE(expected_events.size() != 0);

            std::unique_ptr<Event> expected = std::move(expected_events.front());
            expected_events.pop_front();
            auto actual = dynamic_cast<NoteOffEvent*>(expected.get());

            REQUIRE(actual != nullptr);
            CHECK(actual->dt == dt);
            CHECK(actual->channel == channel);
            CHECK(actual->note == note);
            CHECK(actual->velocity == velocity);
        }

        void polyphonic_key_pressure(uint32_t dt, uint8_t channel, uint8_t note, uint8_t pressure) override
        {
            REQUIRE(expected_events.size() != 0);

            std::unique_ptr<Event> expected = std::move(expected_events.front());
            expected_events.pop_front();
            auto actual = dynamic_cast<PolyphonicEvent*>(expected.get());

            REQUIRE(actual != nullptr);
            CHECK(actual->dt == dt);
            CHECK(actual->channel == channel);
            CHECK(actual->note == note);
            CHECK(actual->pressure == pressure);
        }

        void control_change(uint32_t dt, uint8_t channel, uint8_t controller, uint8_t value) override
        {
            REQUIRE(expected_events.size() != 0);

            std::unique_ptr<Event> expected = std::move(expected_events.front());
            expected_events.pop_front();
            auto actual = dynamic_cast<ControlChangeEvent*>(expected.get());

            REQUIRE(actual != nullptr);
            CHECK(actual->dt == dt);
            CHECK(actual->channel == channel);
            CHECK(actual->controller == controller);
            CHECK(actual->value == value);
        }

        void program_change(uint32_t dt, uint8_t channel, uint8_t program) override
        {
            REQUIRE(expected_events.size() != 0);

            std::unique_ptr<Event> expected = std::move(expected_events.front());
            expected_events.pop_front();
            auto actual = dynamic_cast<ProgramChangeEvent*>(expected.get());

            REQUIRE(actual != nullptr);
            CHECK(actual->dt == dt);
            CHECK(actual->channel == channel);
            CHECK(actual->program == program);
        }

        void channel_pressure(uint32_t dt, uint8_t channel, uint8_t pressure) override
        {
            REQUIRE(expected_events.size() != 0);

            std::unique_ptr<Event> expected = std::move(expected_events.front());
            expected_events.pop_front();
            auto actual = dynamic_cast<ChannelPressureEvent*>(expected.get());

            REQUIRE(actual != nullptr);
            CHECK(actual->dt == dt);
            CHECK(actual->channel == channel);
            CHECK(actual->pressure == pressure);
        }

        void pitch_wheel_change(uint32_t dt, uint8_t channel, uint16_t value) override
        {
            REQUIRE(expected_events.size() != 0);

            std::unique_ptr<Event> expected = std::move(expected_events.front());
            expected_events.pop_front();
            auto actual = dynamic_cast<PitchWheelChangeEvent*>(expected.get());

            REQUIRE(actual != nullptr);
            CHECK(actual->dt == dt);
            CHECK(actual->value == value);
        }

        void meta(uint32_t dt, uint8_t type, std::unique_ptr<char[]> data, int data_size) override
        {
            {
                INFO("This failure means that your function finds nonexistent events");

                REQUIRE(expected_events.size() != 0);
            }

            std::unique_ptr<Event> expected = std::move(expected_events.front());
            expected_events.pop_front();
            auto actual = dynamic_cast<MetaEvent*>(expected.get());

            REQUIRE(actual != nullptr);
            CHECK(actual->dt == dt);
            CHECK(actual->type == type);
            REQUIRE(actual->data_size == data_size);

            for (int i = 0; i != data_size; ++i)
            {
                CHECK(actual->data[i] == data[i]);
            }
        }

        void sysex(uint32_t dt, std::unique_ptr<char[]> data, int data_size) override
        {
            REQUIRE(expected_events.size() != 0);

            std::unique_ptr<Event> expected = std::move(expected_events.front());
            expected_events.pop_front();
            auto actual = dynamic_cast<SysexEvent*>(expected.get());

            REQUIRE(actual != nullptr);
            CHECK(actual->dt == dt);
            REQUIRE(actual->data_size == data_size);

            for (int i = 0; i != data_size; ++i)
            {
                CHECK(actual->data[i] == data[i]);
            }
        }

        void check_finished()
        {
            SECTION("Checking that all events have been handled")
            {
                INFO("This failure means that your code did not find all events in the track");

                CHECK(expected_events.size() == 0);
            }
        }
    };

    class Builder
    {
    private:
        std::list<std::unique_ptr<Event>> expected_events;

    public:
        Builder& note_on(uint32_t dt, uint8_t channel, uint8_t note, uint8_t velocity)
        {
            auto event = std::make_unique<NoteOnEvent>();
            event->dt = dt;
            event->channel = channel;
            event->note = note;
            event->velocity = velocity;

            expected_events.push_back(std::move(event));

            return *this;
        }

        Builder& note_off(uint32_t dt, uint8_t channel, uint8_t note, uint8_t velocity)
        {
            auto event = std::make_unique<NoteOffEvent>();
            event->dt = dt;
            event->channel = channel;
            event->note = note;
            event->velocity = velocity;

            expected_events.push_back(std::move(event));

            return *this;
        }

        Builder& polyphonic_key_pressure(uint32_t dt, uint8_t channel, uint8_t note, uint8_t pressure)
        {
            auto event = std::make_unique<PolyphonicEvent>();
            event->dt = dt;
            event->channel = channel;
            event->note = note;
            event->pressure = pressure;

            expected_events.push_back(std::move(event));

            return *this;
        }

        Builder& control_change(uint32_t dt, uint8_t channel, uint8_t controller, uint8_t value)
        {
            auto event = std::make_unique<ControlChangeEvent>();
            event->dt = dt;
            event->channel = channel;
            event->controller = controller;
            event->value = value;

            expected_events.push_back(std::move(event));

            return *this;
        }

        Builder& program_change(uint32_t dt, uint8_t channel, uint8_t program)
        {
            auto event = std::make_unique<ProgramChangeEvent>();
            event->dt = dt;
            event->channel = channel;
            event->program = program;

            expected_events.push_back(std::move(event));

            return *this;
        }

        Builder& channel_pressure(uint32_t dt, uint8_t channel, uint8_t pressure)
        {
            auto event = std::make_unique<ChannelPressureEvent>();
            event->dt = dt;
            event->channel = channel;
            event->pressure = pressure;

            expected_events.push_back(std::move(event));

            return *this;
        }

        Builder& pitch_wheel_change(uint32_t dt, uint8_t channel, uint16_t value)
        {
            auto event = std::make_unique<PitchWheelChangeEvent>();
            event->dt = dt;
            event->channel = channel;
            event->value = value;

            expected_events.push_back(std::move(event));

            return *this;
        }

        Builder& meta(uint32_t dt, uint8_t type, std::unique_ptr<char[]> data, int data_size)
        {
            auto event = std::make_unique<MetaEvent>();
            event->dt = dt;
            event->type = type;
            event->data = std::move(data);
            event->data_size = data_size;

            expected_events.push_back(std::move(event));
            
            return *this;
        }

        Builder& sysex(uint32_t dt, std::unique_ptr<char[]> data, int data_size)
        {
            auto event = std::make_unique<SysexEvent>();
            event->dt = dt;
            event->data = std::move(data);
            event->data_size = data_size;

            expected_events.push_back(std::move(event));

            return *this;
        }
        
        std::unique_ptr<TestEventReceiver> build()
        {
            return std::make_unique<TestEventReceiver>(std::move(expected_events));
        }
    };
}



TEST_CASE("Reading empty MTrk")
{    
    char buffer[] = { 
        'M', 'T', 'r', 'k',
        0x00, 0x00, 0x00, 0x04, // Length
        0x00, char(0xFF), 0x2F, 0x00 // End of track
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);
    
    auto receiver = Builder().meta(0, 0x2F, nullptr, 0).build();
    REQUIRE(read_mtrk(ss, *receiver));
    receiver->check_finished();
}

TEST_CASE("Reading MTrk with single zero-length meta event with dt=0")
{
    char buffer[] = {
        'M', 'T', 'r', 'k',
        0x00, 0x00, 0x00, 8, // Length
        0x00, char(0xFF), 0x01, 0x00, // Some zero-length meta event
        0x00, char(0xFF), 0x2F, 0x00 // End of track
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .meta(0, 0x01, nullptr, 0)
        .meta(0, 0x2F, nullptr, 0)
        .build();

    REQUIRE(read_mtrk(ss, *receiver));
    receiver->check_finished();
}

TEST_CASE("Reading MTrk with single zero-length meta event with dt=1")
{
    char buffer[] = {
        'M', 'T', 'r', 'k',
        0x00, 0x00, 0x00, 8, // Length
        0x01, char(0xFF), 0x01, 0x00, // Some zero-length meta event
        0x00, char(0xFF), 0x2F, 0x00 // End of track
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .meta(1, 0x01, nullptr, 0)
        .meta(0, 0x2F, nullptr, 0)
        .build();

    REQUIRE(read_mtrk(ss, *receiver));
    receiver->check_finished();
}

TEST_CASE("Reading MTrk with single zero-length meta event with dt=0b1000'0000")
{
    char buffer[] = {
        'M', 'T', 'r', 'k',
        0x00, 0x00, 0x00, 9, // Length
        char(0b1000'0001), 0b0000'0000, char(0xFF), 0x01, 0x00, // Some zero-length meta event
        0x00, char(0xFF), 0x2F, 0x00 // End of track
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .meta(0b1000'0000, 0x01, nullptr, 0)
        .meta(0, 0x2F, nullptr, 0)
        .build();

    REQUIRE(read_mtrk(ss, *receiver));
    receiver->check_finished();
}

TEST_CASE("Reading MTrk with single zero-length meta event with dt=0b1000'0110")
{
    char buffer[] = {
        'M', 'T', 'r', 'k',
        0x00, 0x00, 0x00, 9, // Length
        char(0b1000'0001), 0b0000'0110, char(0xFF), 0x01, 0x00, // Some zero-length meta event
        0x00, char(0xFF), 0x2F, 0x00 // End of track
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .meta(0b1000'0110, 0x01, nullptr, 0)
        .meta(0, 0x2F, nullptr, 0)
        .build();

    REQUIRE(read_mtrk(ss, *receiver));
    receiver->check_finished();
}

TEST_CASE("Reading MTrk with single meta event with data = { 0 }")
{
    char buffer[] = {
        'M', 'T', 'r', 'k',
        0x00, 0x00, 0x00, 9, // Length
        0, char(0xFF), 0x05, 0x01, 0x00,
        0x00, char(0xFF), 0x2F, 0x00 // End of track
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .meta(0, 0x05, std::unique_ptr<char[]>(new char[1] {0}), 1)
        .meta(0, 0x2F, nullptr, 0)
        .build();

    REQUIRE(read_mtrk(ss, *receiver));
    receiver->check_finished();
}

TEST_CASE("Reading MTrk with single meta event with data = { 0x12, 0x34 }")
{
    char buffer[] = {
        'M', 'T', 'r', 'k',
        0x00, 0x00, 0x00, 10, // Length
        0, char(0xFF), 0x05, 0x02, 0x12, 0x34,
        0x00, char(0xFF), 0x2F, 0x00 // End of track
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .meta(0, 0x05, std::unique_ptr<char[]>(new char[2]{ 0x12, 0x34 }), 2)
        .meta(0, 0x2F, nullptr, 0)
        .build();

    REQUIRE(read_mtrk(ss, *receiver));
    receiver->check_finished();
}

TEST_CASE("Reading MTrk with two meta events")
{
    char buffer[] = {
        'M', 'T', 'r', 'k',
        0x00, 0x00, 0x00, 12, // Length
        1, char(0xFF), 0x01, 0x00, // Meta 1
        1, char(0xFF), 0x02, 0x00, // Meta 2
        0x00, char(0xFF), 0x2F, 0x00 // End of track
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .meta(1, 0x01, nullptr, 0)
        .meta(1, 0x02, nullptr, 0)
        .meta(0, 0x2F, nullptr, 0)
        .build();

    REQUIRE(read_mtrk(ss, *receiver));
    receiver->check_finished();
}

TEST_CASE("Reading MTrk with three meta events")
{
    char buffer[] = {
        'M', 'T', 'r', 'k',
        0x00, 0x00, 0x00, 12, // Length
        1, char(0xFF), 0x01, 0x00, // Meta 1
        1, char(0xFF), 0x02, 0x00, // Meta 2
        0x00, char(0xFF), 0x2F, 0x00 // End of track
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .meta(1, 0x01, nullptr, 0)
        .meta(1, 0x02, nullptr, 0)
        .meta(0, 0x2F, nullptr, 0)
        .build();

    REQUIRE(read_mtrk(ss, *receiver));
    receiver->check_finished();
}

TEST_CASE("Reading MTrk with sysex event with no data")
{
    char buffer[] = {
        'M', 'T', 'r', 'k',
        0x00, 0x00, 0x00, 7, // Length
        0, char(0xF0), 0x00, // Sysex
        0x00, char(0xFF), 0x2F, 0x00 // End of track
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .sysex(0, nullptr, 0)
        .meta(0, 0x2F, nullptr, 0)
        .build();

    REQUIRE(read_mtrk(ss, *receiver));
    receiver->check_finished();
}

TEST_CASE("Reading MTrk with sysex event with data = {1, 2, 3}")
{
    char buffer[] = {
        'M', 'T', 'r', 'k',
        0x00, 0x00, 0x00, 10, // Length
        0, char(0xF0), 0x03, 1, 2, 3, // Sysex
        0x00, char(0xFF), 0x2F, 0x00 // End of track
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .sysex(0, std::unique_ptr<char[]>(new char[3]{ 1, 2, 3 }), 3)
        .meta(0, 0x2F, nullptr, 0)
        .build();

    REQUIRE(read_mtrk(ss, *receiver));
    receiver->check_finished();
}

TEST_CASE("Reading MTrk with sysex event at dt = 0b1111'1111")
{
    char buffer[] = {
        'M', 'T', 'r', 'k',
        0x00, 0x00, 0x00, 8, // Length
        char(0b1000'0001), 0b0111'1111, char(0xF0), 0, // Sysex
        0x00, char(0xFF), 0x2F, 0x00 // End of track
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .sysex(0b1111'1111, nullptr, 0)
        .meta(0, 0x2F, nullptr, 0)
        .build();

    REQUIRE(read_mtrk(ss, *receiver));
    receiver->check_finished();
}


TEST_CASE("Reading MTrk with note off event (dt 0 channel 0 note 0 velocity 0)")
{
    char buffer[] = {
        'M', 'T', 'r', 'k',
        0x00, 0x00, 0x00, 8, // Length
        0, char(0b1000'0000), 0, 0, // Note off
        0x00, char(0xFF), 0x2F, 0x00 // End of track
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .note_off(0, 0, 0, 0)
        .meta(0, 0x2F, nullptr, 0)
        .build();

    REQUIRE(read_mtrk(ss, *receiver));
    receiver->check_finished();
}

TEST_CASE("Reading MTrk with note off event (dt 1 channel 0 note 0 velocity 0)")
{
    char buffer[] = {
        'M', 'T', 'r', 'k',
        0x00, 0x00, 0x00, 8, // Length
        1, char(0b1000'0000), 0, 0, // Note off
        0x00, char(0xFF), 0x2F, 0x00 // End of track
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .note_off(1, 0, 0, 0)
        .meta(0, 0x2F, nullptr, 0)
        .build();

    REQUIRE(read_mtrk(ss, *receiver));
    receiver->check_finished();
}

TEST_CASE("Reading MTrk with note off event (dt 0b1'0000000'0000000 channel 0 note 0 velocity 0)")
{
    char buffer[] = {
        'M', 'T', 'r', 'k',
        0x00, 0x00, 0x00, 10, // Length
        char(0b1000'0001), char(0b1000'0000), 0b0000'0000, char(0b1000'0000), 0, 0, // Note off
        0x00, char(0xFF), 0x2F, 0x00 // End of track
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .note_off(0b1'0000000'0000000, 0, 0, 0)
        .meta(0, 0x2F, nullptr, 0)
        .build();

    REQUIRE(read_mtrk(ss, *receiver));
    receiver->check_finished();
}

TEST_CASE("Reading MTrk with note off event (dt 0b1'0000000'0000000 channel 1 note 0 velocity 0)")
{
    char buffer[] = {
        'M', 'T', 'r', 'k',
        0x00, 0x00, 0x00, 10, // Length
        char(0b1000'0001), char(0b1000'0000), 0b0000'0000, char(0b1000'0001), 0, 0, // Note off
        0x00, char(0xFF), 0x2F, 0x00 // End of track
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .note_off(0b1'0000000'0000000, 1, 0, 0)
        .meta(0, 0x2F, nullptr, 0)
        .build();

    REQUIRE(read_mtrk(ss, *receiver));
    receiver->check_finished();
}

TEST_CASE("Reading MTrk with note off event (dt 0b1'0000000'0000000 channel 15 note 0 velocity 0)")
{
    char buffer[] = {
        'M', 'T', 'r', 'k',
        0x00, 0x00, 0x00, 10, // Length
        char(0b1000'0001), char(0b1000'0000), 0b0000'0000, char(0b1000'1111), 0, 0, // Note off
        0x00, char(0xFF), 0x2F, 0x00 // End of track
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .note_off(0b1'0000000'0000000, 15, 0, 0)
        .meta(0, 0x2F, nullptr, 0)
        .build();

    REQUIRE(read_mtrk(ss, *receiver));
    receiver->check_finished();
}

TEST_CASE("Reading MTrk with note off event (dt 0b1'0000000'0000000 channel 15 note 1 velocity 0)")
{
    char buffer[] = {
        'M', 'T', 'r', 'k',
        0x00, 0x00, 0x00, 10, // Length
        char(0b1000'0001), char(0b1000'0000), 0b0000'0000, char(0b1000'1111), 1, 0, // Note off
        0x00, char(0xFF), 0x2F, 0x00 // End of track
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .note_off(0b1'0000000'0000000, 15, 1, 0)
        .meta(0, 0x2F, nullptr, 0)
        .build();

    REQUIRE(read_mtrk(ss, *receiver));
    receiver->check_finished();
}

TEST_CASE("Reading MTrk with note off event (dt 0b1'0000000'0000000 channel 15 note 32 velocity 0)")
{
    char buffer[] = {
        'M', 'T', 'r', 'k',
        0x00, 0x00, 0x00, 10, // Length
        char(0b1000'0001), char(0b1000'0000), 0b0000'0000, char(0b1000'1111), 32, 0, // Note off
        0x00, char(0xFF), 0x2F, 0x00 // End of track
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .note_off(0b1'0000000'0000000, 15, 32, 0)
        .meta(0, 0x2F, nullptr, 0)
        .build();

    REQUIRE(read_mtrk(ss, *receiver));
    receiver->check_finished();
}

TEST_CASE("Reading MTrk with note off event (dt 0b1'0000000'0000000 channel 15 note 32 velocity 100)")
{
    char buffer[] = {
        'M', 'T', 'r', 'k',
        0x00, 0x00, 0x00, 10, // Length
        char(0b1000'0001), char(0b1000'0000), 0b0000'0000, char(0b1000'1111), 32, 100, // Note off
        0x00, char(0xFF), 0x2F, 0x00 // End of track
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .note_off(0b1'0000000'0000000, 15, 32, 100)
        .meta(0, 0x2F, nullptr, 0)
        .build();

    REQUIRE(read_mtrk(ss, *receiver));
    receiver->check_finished();
}

TEST_CASE("Reading MTrk with note on event (dt 0 channel 0 note 0 velocity 0)")
{
    char buffer[] = {
        'M', 'T', 'r', 'k',
        0x00, 0x00, 0x00, 8, // Length
        0, char(0b1001'0000), 0, 0, // Note on
        0x00, char(0xFF), 0x2F, 0x00 // End of track
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .note_on(0, 0, 0, 0)
        .meta(0, 0x2F, nullptr, 0)
        .build();

    REQUIRE(read_mtrk(ss, *receiver));
    receiver->check_finished();
}

TEST_CASE("Reading MTrk with note on event (dt 1 channel 0 note 0 velocity 0)")
{
    char buffer[] = {
        'M', 'T', 'r', 'k',
        0x00, 0x00, 0x00, 8, // Length
        1, char(0b1001'0000), 0, 0, // Note on
        0x00, char(0xFF), 0x2F, 0x00 // End of track
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .note_on(1, 0, 0, 0)
        .meta(0, 0x2F, nullptr, 0)
        .build();

    REQUIRE(read_mtrk(ss, *receiver));
    receiver->check_finished();
}

TEST_CASE("Reading MTrk with note on event (dt 0b1'0000000'0000000 channel 0 note 0 velocity 0)")
{
    char buffer[] = {
        'M', 'T', 'r', 'k',
        0x00, 0x00, 0x00, 10, // Length
        char(0b1000'0001), char(0b1000'0000), 0b0000'0000, char(0b1001'0000), 0, 0, // Note on
        0x00, char(0xFF), 0x2F, 0x00 // End of track
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .note_on(0b1'0000000'0000000, 0, 0, 0)
        .meta(0, 0x2F, nullptr, 0)
        .build();

    REQUIRE(read_mtrk(ss, *receiver));
    receiver->check_finished();
}

TEST_CASE("Reading MTrk with note on event (dt 0b1'0000000'0000000 channel 1 note 0 velocity 0)")
{
    char buffer[] = {
        'M', 'T', 'r', 'k',
        0x00, 0x00, 0x00, 10, // Length
        char(0b1000'0001), char(0b1000'0000), 0b0000'0000, char(0b1001'0001), 0, 0, // Note on
        0x00, char(0xFF), 0x2F, 0x00 // End of track
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .note_on(0b1'0000000'0000000, 1, 0, 0)
        .meta(0, 0x2F, nullptr, 0)
        .build();

    REQUIRE(read_mtrk(ss, *receiver));
    receiver->check_finished();
}

TEST_CASE("Reading MTrk with note on event (dt 0b1'0000000'0000000 channel 15 note 0 velocity 0)")
{
    char buffer[] = {
        'M', 'T', 'r', 'k',
        0x00, 0x00, 0x00, 10, // Length
        char(0b1000'0001), char(0b1000'0000), 0b0000'0000, char(0b1001'1111), 0, 0, // Note on
        0x00, char(0xFF), 0x2F, 0x00 // End of track
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .note_on(0b1'0000000'0000000, 15, 0, 0)
        .meta(0, 0x2F, nullptr, 0)
        .build();

    REQUIRE(read_mtrk(ss, *receiver));
    receiver->check_finished();
}

TEST_CASE("Reading MTrk with note on event (dt 0b1'0000000'0000000 channel 15 note 1 velocity 0)")
{
    char buffer[] = {
        'M', 'T', 'r', 'k',
        0x00, 0x00, 0x00, 10, // Length
        char(0b1000'0001), char(0b1000'0000), 0b0000'0000, char(0b1001'1111), 1, 0, // Note on
        0x00, char(0xFF), 0x2F, 0x00 // End of track
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .note_on(0b1'0000000'0000000, 15, 1, 0)
        .meta(0, 0x2F, nullptr, 0)
        .build();

    REQUIRE(read_mtrk(ss, *receiver));
    receiver->check_finished();
}

TEST_CASE("Reading MTrk with note on event (dt 0b1'0000000'0000000 channel 15 note 32 velocity 0)")
{
    char buffer[] = {
        'M', 'T', 'r', 'k',
        0x00, 0x00, 0x00, 10, // Length
        char(0b1000'0001), char(0b1000'0000), 0b0000'0000, char(0b1001'1111), 32, 0, // Note on
        0x00, char(0xFF), 0x2F, 0x00 // End of track
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .note_on(0b1'0000000'0000000, 15, 32, 0)
        .meta(0, 0x2F, nullptr, 0)
        .build();

    REQUIRE(read_mtrk(ss, *receiver));
    receiver->check_finished();
}

TEST_CASE("Reading MTrk with note on event (dt 0b1'0000000'0000000 channel 15 note 32 velocity 100)")
{
    char buffer[] = {
        'M', 'T', 'r', 'k',
        0x00, 0x00, 0x00, 10, // Length
        char(0b1000'0001), char(0b1000'0000), 0b0000'0000, char(0b1001'1111), 32, 100, // Note on
        0x00, char(0xFF), 0x2F, 0x00 // End of track
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .note_on(0b1'0000000'0000000, 15, 32, 100)
        .meta(0, 0x2F, nullptr, 0)
        .build();

    REQUIRE(read_mtrk(ss, *receiver));
    receiver->check_finished();
}

TEST_CASE("Reading MTrk with polyphonic key pressure event (dt 5 channel 3 note 100 pressure 210)")
{
    char buffer[] = {
        'M', 'T', 'r', 'k',
        0x00, 0x00, 0x00, 8, // Length
        5, char(0b1010'0011), 100, char(210), // Polyphonic key pressure
        0x00, char(0xFF), 0x2F, 0x00 // End of track
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .polyphonic_key_pressure(5, 3, 100, 210)
        .meta(0, 0x2F, nullptr, 0)
        .build();

    REQUIRE(read_mtrk(ss, *receiver));
    receiver->check_finished();
}

TEST_CASE("Reading MTrk with control change event (dt 77 channel 7 controller 3 value 55)")
{
    char buffer[] = {
        'M', 'T', 'r', 'k',
        0x00, 0x00, 0x00, 8, // Length
        77, char(0b1011'0111), 3, 55, // Control change
        0x00, char(0xFF), 0x2F, 0x00 // End of track
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .control_change(77, 7, 3, 55)
        .meta(0, 0x2F, nullptr, 0)
        .build();

    REQUIRE(read_mtrk(ss, *receiver));
    receiver->check_finished();
}

TEST_CASE("Reading MTrk with program change event (dt 127 channel 4 program 2)")
{
    char buffer[] = {
        'M', 'T', 'r', 'k',
        0x00, 0x00, 0x00, 7, // Length
        127, char(0b1100'0100), 2, // Program change
        0x00, char(0xFF), 0x2F, 0x00 // End of track
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .program_change(127, 4, 2)
        .meta(0, 0x2F, nullptr, 0)
        .build();

    REQUIRE(read_mtrk(ss, *receiver));
    receiver->check_finished();
}

TEST_CASE("Reading MTrk with channel pressure event (dt 128 channel 3 pressure 99)")
{
    char buffer[] = {
        'M', 'T', 'r', 'k',
        0x00, 0x00, 0x00, 8, // Length
        char(0b1000'0001), 0b0000'0000, char(0b1101'0011), 99, // Channel pressure
        0x00, char(0xFF), 0x2F, 0x00 // End of track
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .channel_pressure(128, 3, 99)
        .meta(0, 0x2F, nullptr, 0)
        .build();

    REQUIRE(read_mtrk(ss, *receiver));
    receiver->check_finished();
}

TEST_CASE("Reading MTrk with pitch wheel change event (dt 129 channel 15 value 0)")
{
    char buffer[] = {
        'M', 'T', 'r', 'k',
        0x00, 0x00, 0x00, 9, // Length
        char(0b1000'0001), 0b0000'0001, char(0b1110'1111), 0, 0, // Pitch wheel change
        0x00, char(0xFF), 0x2F, 0x00 // End of track
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .pitch_wheel_change(129, 15, 0)
        .meta(0, 0x2F, nullptr, 0)
        .build();

    REQUIRE(read_mtrk(ss, *receiver));
    receiver->check_finished();
}

TEST_CASE("Reading MTrk with pitch wheel change event (dt 129 channel 15 value 5)")
{
    char buffer[] = {
        'M', 'T', 'r', 'k',
        0x00, 0x00, 0x00, 9, // Length
        char(0b1000'0001), 0b0000'0001, char(0b1110'1111), 5, 0, // Pitch wheel change
        0x00, char(0xFF), 0x2F, 0x00 // End of track
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .pitch_wheel_change(129, 15, 5)
        .meta(0, 0x2F, nullptr, 0)
        .build();

    REQUIRE(read_mtrk(ss, *receiver));
    receiver->check_finished();
}

TEST_CASE("Reading MTrk with pitch wheel change event (dt 129 channel 15 value 0x1234)")
{
    char buffer[] = {
        'M', 'T', 'r', 'k',
        0x00, 0x00, 0x00, 9, // Length
        char(0b1000'0001), 0b0000'0001, char(0b1110'1111), 0x34, 0x12, // Pitch wheel change
        0x00, char(0xFF), 0x2F, 0x00 // End of track
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .pitch_wheel_change(129, 15, 0x1234)
        .meta(0, 0x2F, nullptr, 0)
        .build();

    REQUIRE(read_mtrk(ss, *receiver));
    receiver->check_finished();
}

TEST_CASE("Reading MTrk with erroneous length (too small)")
{
    char buffer[] = {
        'M', 'T', 'r', 'k',
        0x00, 0x00, 0x00, 11, // Length
        0, char(0b1001'0000), 0, 0, // Note on
        10, char(0b1000'0000), 0, 0, // Note off
        0x00, char(0xFF), 0x2F, 0x00 // End of track
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .note_on(0, 0, 0, 0)
        .note_off(10, 0, 0, 0)
        .meta(0, 0x2F, nullptr, 0)
        .build();

    REQUIRE(!read_mtrk(ss, *receiver));
}

TEST_CASE("Reading MTrk with erroneous length (too large)")
{
    char buffer[] = {
        'M', 'T', 'r', 'k',
        0x00, 0x00, 0x00, 13, // Length
        0, char(0b1001'0000), 0, 0, // Note on
        10, char(0b1000'0000), 0, 0, // Note off
        0x00, char(0xFF), 0x2F, 0x00 // End of track
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .note_on(0, 0, 0, 0)
        .note_off(10, 0, 0, 0)
        .meta(0, 0x2F, nullptr, 0)
        .build();

    REQUIRE(!read_mtrk(ss, *receiver));
}

TEST_CASE("Reading MTrk, multiple note on events without running status")
{
    char buffer[] = {
        'M', 'T', 'r', 'k',
        0x00, 0x00, 0x00, 12, // Length
        0, char(0b1001'0000), 0, 0, // Note on
        10, char(0b1001'0000), 5, 0, // Note on
        0x00, char(0xFF), 0x2F, 0x00 // End of track
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .note_on(0, 0, 0, 0)
        .note_on(10, 0, 5, 0)
        .meta(0, 0x2F, nullptr, 0)
        .build();

    REQUIRE(read_mtrk(ss, *receiver));
}

TEST_CASE("Reading MTrk, multiple note on events with running status")
{
    char buffer[] = {
        'M', 'T', 'r', 'k',
        0x00, 0x00, 0x00, 11, // Length
        0, char(0b1001'0000), 0, 0, // Note on
        10, 5, 0, // Note on
        0x00, char(0xFF), 0x2F, 0x00 // End of track
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .note_on(0, 0, 0, 0)
        .note_on(10, 0, 5, 0)
        .meta(0, 0x2F, nullptr, 0)
        .build();

    REQUIRE(read_mtrk(ss, *receiver));
}

TEST_CASE("Reading MTrk, multiple note off events with running status")
{
    char buffer[] = {
        'M', 'T', 'r', 'k',
        0x00, 0x00, 0x00, 14, // Length
        0, char(0b1000'0001), 0, 0, // Note off
        10, 5, 0, // Note off
        20, 9, 0, // Note off
        0x00, char(0xFF), 0x2F, 0x00 // End of track
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .note_off(0, 1, 0, 0)
        .note_off(10, 1, 5, 0)
        .note_off(20, 1, 9, 0)
        .meta(0, 0x2F, nullptr, 0)
        .build();

    REQUIRE(read_mtrk(ss, *receiver));
}

TEST_CASE("Reading MTrk consisting of note on, note on, note off, note off")
{
    char buffer[] = {
        'M', 'T', 'r', 'k',
        0x00, 0x00, 0x00, 18, // Length
        0, char(0b1001'0000), 50, char(255), // Note on
        0, 49, char(255), // Note on
        10, char(0b1000'0000), 50, 0, // Note off
        0, 49, 0, // Note off
        0x00, char(0xFF), 0x2F, 0x00 // End of track
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .note_on(0, 0, 50, 255)
        .note_on(0, 0, 49, 255)
        .note_off(10, 0, 50, 0)
        .note_off(0, 0, 49, 0)
        .meta(0, 0x2F, nullptr, 0)
        .build();

    REQUIRE(read_mtrk(ss, *receiver));
    receiver->check_finished();
}

#endif
