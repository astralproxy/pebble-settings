#include "main.h"
#include "dithering.h"
#include <time.h>


static void render_bg(Layer* layer, GContext* ctx) 
{
    APP_LOG(APP_LOG_LEVEL_DEBUG, "render_bg");
}

static void render_time(Layer* layer, GContext* ctx) 
{
    APP_LOG(APP_LOG_LEVEL_DEBUG, "render_time");
#ifdef PBL_COLOR
    graphics_context_set_text_color(ctx, mLightColor);
#else
    graphics_context_set_text_color(ctx, mTextColor);
#endif // PBL_COLOR

    // Draw the time.
    APP_LOG(APP_LOG_LEVEL_DEBUG, "render_time: draw time text");
    
    if (mIs24HourStyle)
    {
        if (mHour < 10)
        {
            graphics_draw_text(ctx, "0", mTimeFontSmall, GRect(TIMEX+TIME_SIZE*2+SMALL_TEXT_OFFSET, TIMEY+(TIME_SIZE-16), 16, 16), GTextOverflowModeTrailingEllipsis, GTextAlignmentRight, NULL);
        }
    }
    else if ((mHour < 10 || (mHour > 12 && mHour < 22)))
    {
        // Draw the leading 0 smaller, then the time.
        APP_LOG(APP_LOG_LEVEL_DEBUG, "render_time: draw leading 0");
        graphics_draw_text(ctx, "0", mTimeFontSmall, GRect(TIMEX+TIME_SIZE*2+SMALL_TEXT_OFFSET, TIMEY+(TIME_SIZE-16), 16, 16), GTextOverflowModeTrailingEllipsis, GTextAlignmentRight, NULL);
    }
    
    graphics_draw_text(ctx, mTimeText, mTimeFont, GRect(TIMEX, TIMEY, TIME_SIZE*5, TIME_SIZE), GTextOverflowModeTrailingEllipsis, GTextAlignmentRight, NULL);

    // Draw the date.
    APP_LOG(APP_LOG_LEVEL_DEBUG, "render_time: draw date text");
    graphics_draw_text(ctx, mDateText, mDateFont, GRect(DATEX, TIMEY-DATE_SIZE-DATE_OFFSETY, DATE_SIZE*5, DATE_SIZE), GTextOverflowModeTrailingEllipsis, GTextAlignmentRight, NULL);
    
    // Draw top text
    if (mIs24HourStyle)
    {
        mTopText = "24 HR";
    }
    else
    {
        mTopText = "12 hr";
    }
    graphics_draw_text(ctx, mTopText, mDayFont, GRect(TOP_TEXT_X, TOP_TEXT_Y, SCREEN_WIDTH, TEXT_SIZE), GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);
}

static void set_time_display(struct tm* t)
{
    APP_LOG(APP_LOG_LEVEL_DEBUG, "set_time_display: mIs24HourStyle: %d", mIs24HourStyle);
    // Set the day
    int size = sizeof("WEDNESDAYn");
    strftime(mDayText, size, "%A", t);
    
    // Convert the day text to uppercase
    for (int i = 0; mDayText[i] != 0; i++)
    {
        if (mDayText[i] >= 'a' && mDayText[i] <= 'z')
        {
            mDayText[i] -= 0x20;
        }
    }

    mHour = t->tm_hour;
    APP_LOG(APP_LOG_LEVEL_DEBUG, "set_time_display: hour: %d", mHour);
    
    size = sizeof("00:00n");    
	if (mIs24HourStyle)
	{
		strftime(mTimeText, size, "%H:%M", t);
        
        if (mHour < 10)
        {
            // Increment the char pointer to remove the leading 0.
            mTimeText++;
        }
	}
	else
	{
		strftime(mTimeText, size, "%I:%M", t);
        
        if (mHour < 10 || (mHour > 12 && mHour < 22))
        {
            // Increment the char pointer to remove the leading 0.
            mTimeText++;
        }
	}
    APP_LOG(APP_LOG_LEVEL_DEBUG, "set_time_display: mTimeText: %s", mTimeText);
    
    strftime(mDateText, size, "%m.%d", t);
    mMonth = t->tm_mon; // since January, 0-11
    APP_LOG(APP_LOG_LEVEL_DEBUG, "set_time_display: month: %d", mMonth);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "set_time_display: mDateText: %s", mDateText);
}

static void tick_handler(struct tm* t, TimeUnits units_changed)
{
    APP_LOG(APP_LOG_LEVEL_DEBUG, "tick_handler");
	set_time_display(t);
    layer_mark_dirty(sTimeLayer);
}

static void bt_handler(bool connected)
{
    APP_LOG(APP_LOG_LEVEL_DEBUG, "bt_handler. connected: %d", connected);
}

static void battery_handler(BatteryChargeState charge_state)
{
    APP_LOG(APP_LOG_LEVEL_DEBUG, "battery_handler. charging: %d, charge: %d", charge_state.is_charging, charge_state.charge_percent);
    mIsCharging = charge_state.is_charging;
}

static void tap_handler(AccelAxisType axis, int32_t dir)
{
    APP_LOG(APP_LOG_LEVEL_DEBUG, "tap_handler");
}

static void inbox_received_handler(DictionaryIterator* iter, void* context)
{
    APP_LOG(APP_LOG_LEVEL_DEBUG, "inbox_received_handler");

#ifdef PBL_COLOR
    // Apply the colors if available.
    Tuple* color_red_t = dict_find(iter, KEY_COLOR_RED1);
    Tuple* color_green_t = dict_find(iter, KEY_COLOR_GREEN1);
    Tuple* color_blue_t = dict_find(iter, KEY_COLOR_BLUE1);
    Tuple* color_red_t2 = dict_find(iter, KEY_COLOR_RED2);
    Tuple* color_green_t2 = dict_find(iter, KEY_COLOR_GREEN2);
    Tuple* color_blue_t2 = dict_find(iter, KEY_COLOR_BLUE2);
    if ((color_red_t && color_green_t && color_blue_t) &&
        (color_red_t2 && color_green_t2 && color_blue_t2))
    {
        int red = color_red_t->value->int32;
        int green = color_green_t->value->int32;
        int blue = color_blue_t->value->int32;

        // Persist values
        persist_write_int(KEY_COLOR_RED1, red);
        persist_write_int(KEY_COLOR_GREEN1, green);
        persist_write_int(KEY_COLOR_BLUE1, blue);

        mDarkColor = GColorFromRGB(red, green, blue);
        APP_LOG(APP_LOG_LEVEL_DEBUG, "inbox_received_handler: mainColor: R:%d, G:%d, B:%d", red, green, blue);
     
        int red2 = color_red_t2->value->int32;
        int green2 = color_green_t2->value->int32;
        int blue2 = color_blue_t2->value->int32;

        // Persist values
        persist_write_int(KEY_COLOR_RED2, red2);
        persist_write_int(KEY_COLOR_GREEN2, green2);
        persist_write_int(KEY_COLOR_BLUE2, blue2);

        mLightColor = GColorFromRGB(red2, green2, blue2);
        APP_LOG(APP_LOG_LEVEL_DEBUG, "inbox_received_handler: secondaryColor: R:%d, G:%d, B:%d", red2, green2, blue2);
        
        // Prevent stupid user settings from ruining the watch.
        if (mLightColor.r == mDarkColor.r && mLightColor.g == mDarkColor.g && mLightColor.b == mDarkColor.b)
        {
            // Just use white.
            mLightColor = GColorWhite;
            mMediumColor = GColorLightGray;
        }
        else
        {
            // Medium color should be between the other colors.
            mMediumColor = GColorFromRGB((red + red2)     / 2,
                                         (green + green2) / 2,
                                         (blue + blue2)   / 2);
        }

        // Redraw the background.
        layer_mark_dirty(sBgLayer);
    }
#endif // PBL_COLOR
    
    Tuple* twenty_four_t = dict_find(iter, KEY_TWENTY_FOUR);
    if (twenty_four_t)
    {
        mIs24HourStyle = (twenty_four_t->value->int32 > 0);
        persist_write_bool(KEY_TWENTY_FOUR, mIs24HourStyle);
        APP_LOG(APP_LOG_LEVEL_DEBUG, "inbox_received_handler: data: %ld, mIs24HourStyle: %d", twenty_four_t->value->int32, mIs24HourStyle);

        struct tm* t;
        time_t temp;
        temp = time(NULL);
        t = localtime(&temp);
        set_time_display(t);

        layer_mark_dirty(sTimeLayer);
    }
}

static void window_load(Window* window)
{
    // Handle the settings.
#ifdef PBL_COLOR
    // Set default colors.
    mLightColor = GColorCeleste;
    mMediumColor = GColorJaegerGreen;
    mDarkColor = GColorDarkGreen;
    mBackgroundColor = GColorBlack;

    // Try to get saved colors.
    // Colors are auto mapped to the nearest color out of 64 available.
    APP_LOG(APP_LOG_LEVEL_DEBUG, "KEY_COLOR_ 1");
    if (persist_exists(KEY_COLOR_RED1) && persist_exists(KEY_COLOR_GREEN1) && persist_exists(KEY_COLOR_BLUE1))
    {
        int red = persist_read_int(KEY_COLOR_RED1);
        int green = persist_read_int(KEY_COLOR_GREEN1);
        int blue = persist_read_int(KEY_COLOR_BLUE1);
        mDarkColor = GColorFromRGB(red, green, blue);
    }

    APP_LOG(APP_LOG_LEVEL_DEBUG, "KEY_COLOR_ 2");
    if (persist_exists(KEY_COLOR_RED2) && persist_exists(KEY_COLOR_GREEN2) && persist_exists(KEY_COLOR_BLUE2))
    {
        int red = persist_read_int(KEY_COLOR_RED2);
        int green = persist_read_int(KEY_COLOR_GREEN2);
        int blue = persist_read_int(KEY_COLOR_BLUE2);
        mLightColor = GColorFromRGB(red, green, blue);
    }
#else
    mBackgroundColor = GColorBlack;
    mTextColor = GColorWhite;
#endif // PBL_COLOR
    
    APP_LOG(APP_LOG_LEVEL_DEBUG, "KEY_TWENTY_FOUR");
    if (persist_exists(KEY_TWENTY_FOUR))
    {
        mIs24HourStyle = persist_read_bool(KEY_TWENTY_FOUR);
        APP_LOG(APP_LOG_LEVEL_DEBUG, "KEY_TWENTY_FOUR: %d", mIs24HourStyle);
    }
    
    APP_LOG(APP_LOG_LEVEL_DEBUG, "window_load: set_background");
	window_set_background_color(window, mBackgroundColor);
    
    APP_LOG(APP_LOG_LEVEL_DEBUG, "window_load: add layers");
    sBgLayer = layer_create(GRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
	layer_set_update_proc(sBgLayer, (LayerUpdateProc)render_bg);
	layer_add_child(window_get_root_layer(window), sBgLayer);
    
	sTimeLayer = layer_create(GRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
	layer_set_update_proc(sTimeLayer, (LayerUpdateProc)render_time);
	layer_add_child(window_get_root_layer(window), sTimeLayer);

    APP_LOG(APP_LOG_LEVEL_DEBUG, "window_load: setup fonts");
    mDayFont = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_CONSOLAB_14));
	mTimeFont = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DSEG_BOLD_30));
    mTimeFontSmall = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DSEG_BOLD_16));
    mDateFont = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DSEG_NORMAL_18));
    mDateFontSmall = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DSEG_NORMAL_10));

    mDayText = malloc(9+1); // WEDNESDAY
    mTimeText = malloc(5+1); // HH:MM
    mTopText = malloc(5+1); // XX HR
    mDateText = malloc(5+1); // MM.DD

    APP_LOG(APP_LOG_LEVEL_DEBUG, "window_load: set time");
    struct tm* t;
	time_t temp;
	temp = time(NULL);
	t = localtime(&temp);
	set_time_display(t);
    
    //  Init the BT and battery indicators.
    APP_LOG(APP_LOG_LEVEL_DEBUG, "window_load: setup bt and battery");
    bt_handler(bluetooth_connection_service_peek());
    battery_handler(battery_state_service_peek());
}

static void window_unload(Window *window) 
{
    app_timer_cancel(timer);
    layer_destroy(sTimeLayer);
    layer_destroy(sBgLayer);
}

static void init(void)
{
    sWindow = window_create();
    window_set_window_handlers(sWindow, (WindowHandlers) 
    {
		.load = window_load,
		.unload = window_unload,
    });
    
    APP_LOG(APP_LOG_LEVEL_DEBUG, "init: app message open");
    app_message_register_inbox_received(inbox_received_handler);
    app_message_open(100, 100);
    
    APP_LOG(APP_LOG_LEVEL_DEBUG, "init: subscribe handlers");
    bluetooth_connection_service_subscribe(bt_handler);
    battery_state_service_subscribe(battery_handler);
    accel_tap_service_subscribe(tap_handler);
	
    tick_timer_service_subscribe(MINUTE_UNIT, (TickHandler)tick_handler);
    
    window_stack_push(sWindow, true);
}

static void deinit(void) 
{
    //free(mDayText);
    //free(mTimeText);
    //free(mTopText);
    //free(mDateText);

    tick_timer_service_unsubscribe();
    
    window_destroy(sWindow);
}

int main(void) 
{
    init();
    app_event_loop();
    deinit();
}