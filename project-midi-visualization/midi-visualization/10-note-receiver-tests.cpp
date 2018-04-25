#include "settings.h"

#ifdef TEST_BUILD

#include "midi.h"
#include "Catch.h"
#include <vector>


TEST_CASE("NoteFilter (channel 0) with single note (channel 0, number 5, from 0, duration 100)")
{
    std::vector<NOTE> notes;

    NoteFilter filter(0, [&notes](const NOTE& note) {
        notes.push_back(note);
    });

    filter.note_on(0, 0, 5, 255);
    filter.note_off(100, 0, 5, 255);

    REQUIRE(notes.size() == 1);

    CHECK(notes[0] == NOTE{ 0, 5, 0, 100 });
}

TEST_CASE("NoteFilter (channel 2) with single note (channel 2, number 5, from 0, duration 100)")
{
    std::vector<NOTE> notes;

    NoteFilter filter(2, [&notes](const NOTE& note) {
        notes.push_back(note);
    });

    filter.note_on(0, 2, 5, 255);
    filter.note_off(100, 2, 5, 255);

    REQUIRE(notes.size() == 1);

    CHECK(notes[0] == NOTE{ 2, 5, 0, 100 });
}

TEST_CASE("NoteFilter (channel 2) with single note (channel 0, number 5, from 0, duration 100)")
{
    std::vector<NOTE> notes;

    NoteFilter filter(2, [&notes](const NOTE& note) {
        notes.push_back(note);
    });

    filter.note_on(0, 0, 5, 255);
    filter.note_off(100, 0, 5, 255);

    REQUIRE(notes.size() == 0);
}

TEST_CASE("NoteFilter with two consecutive notes")
{
    std::vector<NOTE> notes;

    NoteFilter filter(0, [&notes](const NOTE& note) {
        notes.push_back(note);
    });

    filter.note_on(0, 0, 5, 255);
    filter.note_off(100, 0, 5, 255);
    filter.note_on(100, 0, 7, 255);
    filter.note_off(100, 0, 7, 255);

    REQUIRE(notes.size() == 2);
    CHECK(notes[0] == NOTE{0, 5, 0, 100});
    CHECK(notes[1] == NOTE{ 0, 7, 200, 100 });
}

TEST_CASE("NoteFilter with ABab")
{
    std::vector<NOTE> notes;

    NoteFilter filter(0, [&notes](const NOTE& note) {
        notes.push_back(note);
    });

    filter.note_on(100, 0, 10, 255);
    filter.note_on(200, 0, 15, 255);
    filter.note_off(300, 0, 10, 255);
    filter.note_off(400, 0, 15, 255);

    REQUIRE(notes.size() == 2);
    CHECK(notes[0] == NOTE{ 0, 10, 100, 500 });
    CHECK(notes[1] == NOTE{ 0, 15, 300, 700 });
}

TEST_CASE("NoteFilter with AabB")
{
    std::vector<NOTE> notes;

    NoteFilter filter(0, [&notes](const NOTE& note) {
        notes.push_back(note);
    });

    filter.note_on(100, 0, 50, 255);
    filter.note_on(200, 0, 40, 255);
    filter.note_off(400, 0, 40, 255);
    filter.note_off(300, 0, 50, 255);

    REQUIRE(notes.size() == 2);
    CHECK(notes[0] == NOTE{ 0, 40, 300, 400 });
    CHECK(notes[1] == NOTE{ 0, 50, 100, 900 });
}

TEST_CASE("NoteFilter interprets note on event with velocity 0 as a note off")
{
    std::vector<NOTE> notes;

    NoteFilter filter(0, [&notes](const NOTE& note) {
        notes.push_back(note);
    });

    filter.note_on(100, 0, 50, 255);
    filter.note_on(200, 0, 50, 0);

    REQUIRE(notes.size() == 1);
    CHECK(notes[0] == NOTE{ 0, 50, 100, 200 });
}

TEST_CASE("NoteFilter interprets note on event with velocity 0 as a note off (2 notes)")
{
    std::vector<NOTE> notes;

    NoteFilter filter(0, [&notes](const NOTE& note) {
        notes.push_back(note);
    });

    filter.note_on(100, 0, 50, 255);
    filter.note_on(200, 0, 40, 255);
    filter.note_off(400, 0, 40, 0);
    filter.note_off(300, 0, 50, 0);

    REQUIRE(notes.size() == 2);
    CHECK(notes[0] == NOTE{ 0, 40, 300, 400 });
    CHECK(notes[1] == NOTE{ 0, 50, 100, 900 });
}

TEST_CASE("NoteFilter, 5 simultaneous notes")
{
    std::vector<NOTE> notes;

    NoteFilter filter(0, [&notes](const NOTE& note) {
        notes.push_back(note);
    });

    filter.note_on(0, 0, 0, 255);
    filter.note_on(0, 0, 1, 255);
    filter.note_on(0, 0, 2, 255);
    filter.note_on(0, 0, 3, 255);
    filter.note_on(0, 0, 4, 255);

    filter.note_on(1000, 0, 0, 0);
    filter.note_on(0, 0, 1, 0);
    filter.note_on(0, 0, 2, 0);
    filter.note_on(0, 0, 3, 0);
    filter.note_on(0, 0, 4, 0);

    REQUIRE(notes.size() == 5);
    CHECK(notes[0] == NOTE{ 0, 0, 0, 1000 });
    CHECK(notes[1] == NOTE{ 0, 1, 0, 1000 });
    CHECK(notes[2] == NOTE{ 0, 2, 0, 1000 });
    CHECK(notes[3] == NOTE{ 0, 3, 0, 1000 });
    CHECK(notes[4] == NOTE{ 0, 4, 0, 1000 });
}

#endif
