
enum EventType {
	EVENT_DRAW_LINE,
	EVENT_DRAW_TRIANGLE,
	EVENT_DRAW_LINE_TRIANGLE,
	EVENT_DRAW_RECT,
	EVENT_DRAW_LINE_RECT,
	EVENT_SET_TEX_COORDS,
	EVENT_DRAW_RECT_TEXTURE,
	EVENT_DRAW_CIRCLE,
	EVENT_DRAW_LINE_CIRCLE,
	EVENT_SET_COLOR,
	EVENT_LOAD_TEXTURE,
	EVENT_ROTATE,
	EVENT_CLEAR_RECT,
	EVENT_CLEAR_COLOR,

	EVENT_SLEEP,
};
struct LunaEvent {
	EventType type;
	struct {
		float2 pos;
		float2 pos2;
		float2 pos3;
		float2 pos4;
		int texture;
		float amount;
		float2 size;
		float4 color;
	} draw;
	struct {
		double time;
	} sys;
};

LunaEvent _events[256];
int _event_insert_index = 0; // lua thread
int _event_process_index = 0; // engine thread
int _event_count = 0; // both threads

void push_event(LunaEvent event) {
	// todo: critical section
	if (atomic_fetch32(&_event_count) < array_size(_events)) {
		_events[_event_insert_index++] = event;
		_event_insert_index %= array_size(_events);
		atomic_add32(&_event_count, 1);
	} else {
		printf("ran out of event memory! \n");
	}
}
