#pragma once

#include <pebble.h>

#define KEY_COLOR_RED1   0
#define KEY_COLOR_GREEN1 1
#define KEY_COLOR_BLUE1  2
#define KEY_COLOR_RED2   3
#define KEY_COLOR_GREEN2 4
#define KEY_COLOR_BLUE2  5
#define KEY_TWENTY_FOUR  6

// CONSTS
#define SCREEN_WIDTH 144
#define SCREEN_HEIGHT 168
#define SCREEN_CENTER_X 72
#define SCREEN_CENTER_Y 84

#define TIME_SIZE  30
#define TIMEX      -16
#define TIMEY      108
#define DATE_SIZE  18
#define DATEX      44
#define DATE_OFFSETY 6

#define TEXT_SIZE 14
#define TOP_TEXT_X 0
#define TOP_TEXT_Y 0
#define TEXT_BAR_OFFSET_Y 2
#define LINE_OFFSET 4

#define SMALL_TEXT_OFFSET -4

#define DITHER_UNFILLED_BAR 10 // enum value

struct Block
{
    int x;
    int y;
    int width;
    int height;
    bool visible;
};

static Window* sWindow;
static Layer* sBgLayer;
static Layer* sTimeLayer;
static AppTimer* timer;
static GFont mDayFont;
static char* mDayText;
static GFont mTimeFont;
static GFont mTimeFontSmall;
static char* mTimeText;
static GFont mDateFont;
static GFont mDateFontSmall;
static char* mTopText;
static char* mDateText;

GRect bgRect;

GColor mBackgroundColor;
GColor mTextColor;

#if PBL_COLOR
GColor mLightColor;
GColor mMediumColor;
GColor mDarkColor;
#endif // PBL_COLOR

int mHour;
int mMonth;

bool mIs24HourStyle = true;
bool mIsHorizontal = false;
bool mIsInverted = false;
bool mDrawDithers = true;
bool mDrawEllipse = true;
bool mIsCharging = false;
bool mShakeForDate = false;
//bool mSmallDigits = false;