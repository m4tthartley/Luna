
int SDL_LunaEvent; // note: set during init

enum DrawEventType {
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
	EVENT_PRESENT,
};
char *event_type_names[] = {
	"EVENT_DRAW_LINE",
	"EVENT_DRAW_TRIANGLE",
	"EVENT_DRAW_LINE_TRIANGLE",
	"EVENT_DRAW_RECT",
	"EVENT_DRAW_LINE_RECT",
	"EVENT_SET_TEX_COORDS",
	"EVENT_DRAW_RECT_TEXTURE",
	"EVENT_DRAW_CIRCLE",
	"EVENT_DRAW_LINE_CIRCLE",
	"EVENT_SET_COLOR",
	"EVENT_LOAD_TEXTURE",
	"EVENT_ROTATE",
	"EVENT_CLEAR_RECT",
	"EVENT_CLEAR_COLOR",
	"EVENT_PRESENT",
};
enum InputEventType {
	EVENT_NONE,
	EVENT_MOUSE_DOWN,
	EVENT_MOUSE_UP,
	EVENT_MOUSE_MOTION,
	EVENT_MOUSE_WHEEL,
	EVENT_WINDOW_RESIZE,
	// todo: keyboard events
};
struct LunaEvent {
	int type;
	union {
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
		struct {
			int mouse_button;
			int2 mouse_position;
			int amount;
			int2 window_size;
		} input;
	};
};

struct EventQueue {

	LunaEvent events[256];
	int push_index = 0; // pushing thread
	int pull_index = 0; // pulling thread
	int count = 0; 		// both threads

	bool push_event(LunaEvent event) {
		if (atomic_fetch32(&count) < array_size(events)) {
			events[push_index] = event;
			++push_index %= array_size(events);
			atomic_add32(&count, 1);

			return true;
		}

		printf("ran out of event memory! %i \n", event.type);
		return false;
	}

	void push_window_event(LunaEvent event) {
		if (push_event(event)) {
			SDL_Event sdl_event = {};
			sdl_event.type = SDL_LunaEvent;
			// sdl_event.user.code = _event_insert_index;
			SDL_PushEvent(&sdl_event);
		}
	}

	LunaEvent pull_event() {
		if (atomic_fetch32(&count) < 1) {
			// printf("Pulling an event when there ain't none! \n");
			return {};
		}
		LunaEvent e = events[pull_index];
		++pull_index %= array_size(events);
		atomic_sub32(&count, 1);
		return e;
	}

};

EventQueue command_queue;
EventQueue input_queue;
