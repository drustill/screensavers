#ifndef SCREENSAVER_BRIDGE_H
#define SCREENSAVER_BRIDGE_H

#import <Cocoa/Cocoa.h>
#import <ScreenSaver/ScreenSaver.h>

class Screensaver;

// This macro creates a ScreenSaverView subclass that wraps a C++ Screensaver
// Usage in your screensaver's .mm file:
//   #include "my_screensaver.h"
//   SCREENSAVER_MAIN(MyScreensaverView, MyScreensaver)

#define SCREENSAVER_MAIN(ViewClassName, ScreensaverClass)                      \
  @interface ViewClassName : ScreenSaverView {                                 \
    NSOpenGLContext *glContext;                                                \
    NSOpenGLPixelFormat *pixelFormat;                                          \
    ScreensaverClass *screensaver;                                             \
    NSTimeInterval startTime;                                                  \
    NSTimeInterval lastTime;                                                   \
    BOOL isInitialized;                                                        \
  }                                                                            \
  @end                                                                         \
                                                                               \
  @implementation ViewClassName                                                \
                                                                               \
  -(instancetype)initWithFrame : (NSRect)frame isPreview : (BOOL)isPreview {   \
    self = [super initWithFrame:frame isPreview:isPreview];                    \
    if (self) {                                                                \
      [self setAnimationTimeInterval:1.0 / 60.0];                              \
      isInitialized = NO;                                                      \
      screensaver = nullptr;                                                   \
    }                                                                          \
    return self;                                                               \
  }                                                                            \
                                                                               \
  -(void)setupOpenGL {                                                         \
    NSOpenGLPixelFormatAttribute attrs[] = {NSOpenGLPFADoubleBuffer,           \
                                            NSOpenGLPFAOpenGLProfile,          \
                                            NSOpenGLProfileVersion3_2Core,     \
                                            NSOpenGLPFAColorSize,              \
                                            24,                                \
                                            NSOpenGLPFAAlphaSize,              \
                                            8,                                 \
                                            NSOpenGLPFADepthSize,              \
                                            24,                                \
                                            0};                                \
    pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];      \
    glContext = [[NSOpenGLContext alloc] initWithFormat:pixelFormat            \
                                           shareContext:nil];                  \
    [glContext setView:self];                                                  \
    [glContext makeCurrentContext];                                            \
                                                                               \
    /* Load OpenGL functions via glad */                                       \
    if (!gladLoadGL()) {                                                       \
      NSLog(@"Failed to initialize GLAD");                                     \
      return;                                                                  \
    }                                                                          \
                                                                               \
    NSRect bounds = [self bounds];                                             \
    glViewport(0, 0, bounds.size.width, bounds.size.height);                   \
                                                                               \
    /* Get resource path from bundle */                                        \
    NSBundle *bundle = [NSBundle bundleForClass:[self class]];                 \
    NSString *resourcePath = [bundle resourcePath];                            \
    std::string resPath =                                                      \
        resourcePath ? [resourcePath UTF8String] : std::string("");            \
    if (!resPath.empty() && resPath.back() != '/') {                           \
      resPath += "/";                                                          \
    }                                                                          \
                                                                               \
    screensaver = new ScreensaverClass();                                      \
    screensaver->init(bounds.size.width, bounds.size.height, resPath);         \
                                                                               \
    startTime = [NSDate timeIntervalSinceReferenceDate];                       \
    lastTime = startTime;                                                      \
    isInitialized = YES;                                                       \
  }                                                                            \
                                                                               \
  -(void)startAnimation {                                                      \
    [super startAnimation];                                                    \
    if (!isInitialized) {                                                      \
      [self setupOpenGL];                                                      \
    }                                                                          \
  }                                                                            \
                                                                               \
  -(void)stopAnimation {                                                       \
    [super stopAnimation];                                                     \
    if (screensaver) {                                                         \
      screensaver->cleanup();                                                  \
      delete screensaver;                                                      \
      screensaver = nullptr;                                                   \
    }                                                                          \
    glContext = nil;                                                           \
    pixelFormat = nil;                                                         \
    isInitialized = NO;                                                        \
  }                                                                            \
                                                                               \
  -(void)animateOneFrame {                                                     \
    if (!isInitialized || !screensaver) {                                      \
      return;                                                                  \
    }                                                                          \
                                                                               \
    [glContext makeCurrentContext];                                            \
                                                                               \
    NSTimeInterval currentTime = [NSDate timeIntervalSinceReferenceDate];      \
    float deltaTime = (float)(currentTime - lastTime);                         \
    float totalTime = (float)(currentTime - startTime);                        \
    lastTime = currentTime;                                                    \
                                                                               \
    screensaver->update(deltaTime, totalTime);                                 \
    screensaver->render();                                                     \
                                                                               \
    [glContext flushBuffer];                                                   \
  }                                                                            \
                                                                               \
  -(void)setFrameSize : (NSSize)newSize {                                      \
    [super setFrameSize:newSize];                                              \
    if (isInitialized && screensaver) {                                        \
      [glContext makeCurrentContext];                                          \
      [glContext update];                                                      \
      glViewport(0, 0, newSize.width, newSize.height);                         \
      screensaver->resize(newSize.width, newSize.height);                      \
    }                                                                          \
  }                                                                            \
                                                                               \
  -(BOOL)hasConfigureSheet {                                                   \
    return NO;                                                                 \
  }                                                                            \
                                                                               \
  -(NSWindow *)configureSheet {                                                \
    return nil;                                                                \
  }                                                                            \
                                                                               \
  @end

#endif
