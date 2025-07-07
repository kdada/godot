import plugin, { SimplifiedKeyEvent, SimplifiedMouseEvent } from 'libentry.so';
import { resourceManager } from '@kit.LocalizationKit';
import { display } from '@kit.ArkUI';
import { common } from '@kit.AbilityKit';

class PluginXComponentController extends XComponentController {
  ctx: common.UIAbilityContext
  resMgr: resourceManager.ResourceManager
  windowId: number

  constructor(context: Context, resourceManager: resourceManager.ResourceManager) {
    super()
    this.ctx = context as common.UIAbilityContext
    this.resMgr = resourceManager
    this.windowId = this.ctx.windowStage.getMainWindowSync()
      .getWindowProperties()
      .id
  }

  onSurfaceCreated(surfaceId: string): void {
    plugin.setResourceManager(this.resMgr)
    plugin.setWindowId(this.windowId)
    plugin.setSurfaceId(BigInt(surfaceId))
  }

  onSurfaceChanged(surfaceId: string, rect: SurfaceRect): void {
    plugin.changeSurface(BigInt(surfaceId), rect.surfaceWidth, rect.surfaceHeight)
  }

  onSurfaceDestroyed(surfaceId: string): void {
    plugin.destroySurface(BigInt(surfaceId))
  }
}

@Entry
@Component
struct Index {
  resMgr = this.getUIContext().getHostContext()!.resourceManager
  controller: XComponentController = new PluginXComponentController(getContext(this), this.resMgr)

  convertMouseButton(button: MouseButton) {
    switch (button) {
      case MouseButton.Left:
        return 1;
      case MouseButton.Right:
        return 2;
      case MouseButton.Middle:
        return 3;
      case MouseButton.Forward:
        return 8;
      case MouseButton.Back:
        return 9;
    }
    return 0;
  }

  convertMouseAction(action: MouseAction) {
    switch (action) {
      case MouseAction.Press:
        return 0;
      case MouseAction.Release:
        return 1;
    }
    return 2;
  }

  build() {
    Row() {
      XComponent({
        id: "window",
        type: XComponentType.SURFACE,
        controller: this.controller,
      })
        .onLoad(() => {
          display.getAllDisplays((err, data: Array<display.Display>) => {
            let screenWidth: number = data[0].width
            let screenHeight: number = data[0].height
            let surfaceRect: SurfaceRect = {
              offsetX: 0,
              offsetY: 0,
              surfaceWidth: screenWidth,
              surfaceHeight: screenHeight
            }
            this.controller.setXComponentSurfaceRect(surfaceRect);
            plugin.setup()
          })
        })
        .onTouch((event) => {
          let events: plugin.SimplifiedTouchEvent[] = [];
          let ctx = this.getUIContext()
          for (let index = 0; index < event.touches.length; index++) {
            const touch = event.touches[index];
            events.push({
              type: touch.type,
              id: touch.id,
              x: ctx.vp2px(touch.x),
              y: ctx.vp2px(touch.y),
            })
          }
          plugin.inputTouch(events);
        })
        .onKeyEvent((event) => {
          let keyEvent: SimplifiedKeyEvent = {
            code: event.keyCode,
            unicode: event.unicode ? event.unicode! : 0,
            pressed: event.type == KeyType.Down,
            alt: event.getModifierKeyState ? event.getModifierKeyState(['alt']) : false,
            ctrl: event.getModifierKeyState ? event.getModifierKeyState(['ctrl']) : false,
            shift: event.getModifierKeyState ? event.getModifierKeyState(['shift']) : false,
            meta: event.metaKey != 0,
          };
          plugin.inputKey(keyEvent);
        })
        .onMouse((event) => {
          let ctx = this.getUIContext()
          let mouseEvent: SimplifiedMouseEvent = {
            type: this.convertMouseAction(event.action),
            button: this.convertMouseButton(event.button),
            mask: 0,
            x: ctx.vp2px(event.x),
            y: ctx.vp2px(event.y),
          };
          if (event.pressedButtons) {
            for (let index = 0; index < event.pressedButtons.length; index++) {
              mouseEvent.mask |= 1 << this.convertMouseButton(event.pressedButtons[index])
            }
          }
          plugin.inputMouse(mouseEvent);
        })
        .onAxisEvent((event) => {
          if (!event.scrollStep) {
            return;
          }
          let ctx = this.getUIContext()
          let value = event.getVerticalAxisValue()
          if (value != 0) {
            let mouseEvent: SimplifiedMouseEvent = {
              type: this.convertMouseAction(MouseAction.Release),
              button: value > 0 ? 5 : 4, // 5: WHEEL_DOWN, 4: WHEEL_UP
              mask: 0,
              x: ctx.vp2px(event.x),
              y: ctx.vp2px(event.y),
            };
            plugin.inputMouse(mouseEvent);
          }
          value = event.getHorizontalAxisValue()
          if (value != 0) {
            let mouseEvent: SimplifiedMouseEvent = {
              type: this.convertMouseAction(MouseAction.Release),
              button: value > 0 ? 7 : 6, // 7: WHEEL_RIGHT, 4: WHEEL_LEFT
              mask: 0,
              x: ctx.vp2px(event.x),
              y: ctx.vp2px(event.y),
            };
            plugin.inputMouse(mouseEvent);
          }
        })
        .width('100%')
        .height('100%')
        .expandSafeArea([SafeAreaType.SYSTEM])
    }
    .width('100%')
    .height('100%')
    .expandSafeArea([SafeAreaType.SYSTEM])
  }
}
