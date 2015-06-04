#include <gtest/gtest.h>

#include <QGuiApplication>
#include "ui/displayprofile.h"
#include "ui/targetdisplay.h"
#include "ui/actualdisplay.h"

TEST(TargetDisplay, CalculatePhysicalDimensions)
{
    TargetDisplay display(1920, 1080, 10, 20);
    EXPECT_EQ(display.widthPixels(), 1920);
    EXPECT_EQ(display.heightPixels(), 1080);
    EXPECT_EQ(display.xdpi(), 10);
    EXPECT_EQ(display.ydpi(), 20);
    EXPECT_EQ(display.widthInches(), 192);
    EXPECT_EQ(display.heightInches(), 54);

    Display* d = &display;
    EXPECT_EQ(d->widthPixels(), 1920);
    EXPECT_EQ(d->heightPixels(), 1080);
    EXPECT_EQ(d->xdpi(), 10);
    EXPECT_EQ(d->ydpi(), 20);
    EXPECT_EQ(d->widthInches(), 192);
    EXPECT_EQ(d->heightInches(), 54);

}

// TODO: we should split gberry-lib to gui and non-gui part.
//       now non-gui has also gui dep
/* For command line there is no screens
TEST(ActualDisplay, CalculatePhysicalDimensions)
{
    ActualDisplay display(QGuiApplication::screens().at(0));
    EXPECT_TRUE(display.widthPixels() > 0);
    EXPECT_TRUE(display.heightPixels() > 0);
    EXPECT_TRUE(display.xdpi() > 0);
    EXPECT_TRUE(display.ydpi() > 0);
    EXPECT_EQ(display.widthInches(), display.widthPixels() * display.xdpi());
    EXPECT_EQ(display.heightInches(), display.heightPixels() * display.ydpi());
}*/


TEST(DisplayProfile, WithoutTargetDisplayNoScaling)
{
    TargetDisplay usedDisplay(1920, 1080, 10, 20);

    // TODO: why defaults are provided although targetdisplay ... they are defaults

    // TODO: memory mananement, weak pointers

    // define 'usedDisplay' because otherwise it takes default QScreen
    DisplayProfile profile(1920, 1080, DisplayProfile::PixelMatch, 1.0, &usedDisplay);

    // no target display defined -> uses defaults provided in constructor
    EXPECT_EQ(profile.windowWidth(), 1920);
    EXPECT_EQ(profile.windowHeight(), 1080);
    EXPECT_EQ(profile.sizeScaleFactor(), 1.0);
    EXPECT_EQ(profile.mode(), DisplayProfile::PixelMatch);

    // dpis come from used display
    EXPECT_EQ(profile.xdpi(), usedDisplay.xdpi());
    EXPECT_EQ(profile.ydpi(), usedDisplay.ydpi());

    // case 1: 1:1 match
    // case 2: 2:1 match;
}

TEST(DisplayProfile, WithoutTargetDisplayWithScaling)
{
    TargetDisplay usedDisplay(1920, 1080, 10, 20);
    // define 'usedDisplay' because otherwise it takes default QScreen
    DisplayProfile profile(1920, 1080, DisplayProfile::PixelMatch, 0.5, &usedDisplay);

    // no target display defined -> uses defaults provided in constructor
    EXPECT_EQ(profile.windowWidth(), 1920 / 2);
    EXPECT_EQ(profile.windowHeight(), 1080 / 2);
    EXPECT_EQ(profile.sizeScaleFactor(), 0.5);
    EXPECT_EQ(profile.mode(), DisplayProfile::PixelMatch);

    // dpis come from used display
    EXPECT_EQ(profile.xdpi(), usedDisplay.xdpi() / 2);
    EXPECT_EQ(profile.ydpi(), usedDisplay.ydpi() / 2);
}

TEST(DisplayProfile, WithoutTargetDisplayDimensionMatch)
{
    // if not 'targetDisplay' then DimensionMatch doesn't have effect
    TargetDisplay usedDisplay(1920, 1080, 10, 20);
    // define 'usedDisplay' because otherwise it takes default QScreen
    DisplayProfile profile(1920, 1080, DisplayProfile::DimensionMatch, 0.5, &usedDisplay);

    // no target display defined -> uses defaults provided in constructor
    EXPECT_EQ(profile.windowWidth(), 1920 / 2);
    EXPECT_EQ(profile.windowHeight(), 1080 / 2);
    EXPECT_EQ(profile.sizeScaleFactor(), 0.5);
    EXPECT_EQ(profile.mode(), DisplayProfile::DimensionMatch);

    // dpis come from used display
    EXPECT_EQ(profile.xdpi(), usedDisplay.xdpi() / 2);
    EXPECT_EQ(profile.ydpi(), usedDisplay.ydpi() / 2);
}

TEST(DisplayProfile, TargetDisplayWithPixelMatch)
{
    TargetDisplay usedDisplay(1600, 1400, 30, 20);
    // define 'usedDisplay' because otherwise it takes default QScreen
    // screen x,y defaults shouldn't be used at all when target display is in use
    DisplayProfile profile(640, 320, DisplayProfile::PixelMatch, 0.5, &usedDisplay);

    TargetDisplay target(1920, 1080, 15, 10);
    profile.useTargetDisplay(&target);

    EXPECT_EQ(profile.windowWidth(), 1920 / 2);
    EXPECT_EQ(profile.windowHeight(), 1080 / 2);
    EXPECT_EQ(profile.sizeScaleFactor(), 0.5);
    EXPECT_EQ(profile.mode(), DisplayProfile::PixelMatch);

    // dpis come now from 'targetDisplay'
    EXPECT_EQ(profile.xdpi(), target.xdpi() / 2);
    EXPECT_EQ(profile.ydpi(), target.ydpi() / 2);
}


TEST(DisplayProfile, TargetDisplayWithDimensionMatch)
{
    TargetDisplay usedDisplay(1600, 1400, 30, 20);
    // define 'usedDisplay' because otherwise it takes default QScreen
    // screen x,y defaults shouldn't be used at all when target display is in use
    DisplayProfile profile(640, 320, DisplayProfile::DimensionMatch, 0.1, &usedDisplay);

    TargetDisplay target(1920, 1080, 15, 10);
    profile.useTargetDisplay(&target);

    EXPECT_EQ(profile.windowWidth(), 1920 / 15 * 30 * 0.1);
    EXPECT_EQ(profile.windowHeight(), 1080 / 10 * 20 * 0.1);
    EXPECT_EQ(profile.sizeScaleFactor(), 0.1);
    EXPECT_EQ(profile.mode(), DisplayProfile::DimensionMatch);

    // dpis come now from 'targetDisplay'
    EXPECT_EQ(profile.xdpi(), usedDisplay.xdpi() * 0.1);
    EXPECT_EQ(profile.ydpi(), usedDisplay.ydpi() * 0.1);
}

TEST(DisplayProfile, TargetDisplayWithDimensionMatch_NexusS)
{
    TargetDisplay usedDisplay(1440, 900, 120.8, 120.8);
    // define 'usedDisplay' because otherwise it takes default QScreen
    // screen x,y defaults shouldn't be used at all when target display is in use
    DisplayProfile profile(640, 320, DisplayProfile::DimensionMatch, 1.0, &usedDisplay);

    TargetDisplay target(480, 800, 223, 223);
    profile.useTargetDisplay(&target);

    EXPECT_EQ(profile.windowWidth(), (int) (480 / (double) 223 * 120.8 * 1.0));
    EXPECT_EQ(profile.windowHeight(), (int) (800 / (double) 223 * 120.8 * 1.0));
    EXPECT_EQ(profile.sizeScaleFactor(), 1.0);
    EXPECT_EQ(profile.mode(), DisplayProfile::DimensionMatch);

    // dpis come now from 'targetDisplay'
    EXPECT_EQ(profile.xdpi(), usedDisplay.xdpi() * 1.0);
    EXPECT_EQ(profile.ydpi(), usedDisplay.ydpi() * 1.0);
}


TEST(DisplayProfile, TargetDisplayAutoScaleWithPixelMatch)
{
    TargetDisplay usedDisplay(1600, 1400, 30, 20);
    // define 'usedDisplay' because otherwise it takes default QScreen
    // screen x,y defaults shouldn't be used at all when target display is in use
    // 0.0 means auto scale
    DisplayProfile profile(640, 320, DisplayProfile::PixelMatch, 0.0, &usedDisplay);

    // smallest dimension defines
    EXPECT_EQ(profile.windowWidth(), 1600);
    EXPECT_EQ(profile.windowHeight(), 320 * 1600/640);
    EXPECT_EQ(profile.sizeScaleFactor(), 1600 / (double) 640);
    EXPECT_EQ(profile.mode(), DisplayProfile::PixelMatch);

    // defining target display recalculates scaling
    TargetDisplay target(1920, 1080, 15, 10);
    profile.useTargetDisplay(&target);

    EXPECT_EQ(profile.windowWidth(), 1600);
    EXPECT_EQ(profile.windowHeight(), 1080 * 1600/1920);
    EXPECT_EQ(profile.sizeScaleFactor(), 1600 / (double) 1920);
    EXPECT_EQ(profile.mode(), DisplayProfile::PixelMatch);

}


TEST(DisplayProfile, TargetDisplayAutoScaleWithDimensionMatch)
{
    TargetDisplay usedDisplay(1600, 1400, 30, 20);
    // define 'usedDisplay' because otherwise it takes default QScreen
    // screen x,y defaults shouldn't be used at all when target display is in use
    DisplayProfile profile(640, 320, DisplayProfile::DimensionMatch, 0.0, &usedDisplay);

    TargetDisplay target(1920, 1080, 15, 10);
    profile.useTargetDisplay(&target);

    EXPECT_EQ(profile.windowWidth(), 1600);
    EXPECT_EQ(profile.sizeScaleFactor(), (1600 / (double) 30)  / (1920 / (double) 15));
    EXPECT_EQ(profile.windowHeight(), 1080 / 15 * 30 *  profile.sizeScaleFactor());
    EXPECT_EQ(profile.mode(), DisplayProfile::DimensionMatch);
}

// TEST: limiting largest size

// TEST: env vars
