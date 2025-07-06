import plugin from 'libentry.so';
import { resourceManager } from '@kit.LocalizationKit';
import { display } from '@kit.ArkUI';
import { common, UIAbility } from '@kit.AbilityKit';

class PluginXComponentController extends XComponentController {
  ctx: common.UIAbilityContext
  resMgr: resourceManager.ResourceManager
  windowId: number

  constructor(context: Context, resourceManager: resourceManager.ResourceManager) {
    super()
    this.ctx = context as common.UIAbilityContext
    this.resMgr = resourceManager
    this.windowId = this.ctx.windowStage.getMainWindowSync().getWindowProperties().id
  }

  onSurfaceCreated(surfaceId: string): void {
    plugin.setResourceManager(this.resMgr)
    plugin.setWindowId(this.windowId)
    plugin.setSurfaceId(BigInt(surfaceId))
  }

  onSurfaceChanged(surfaceId: string, rect: SurfaceRect): void {
    plugin.changeSurface(BigInt(surfaceId), BigInt(rect.surfaceWidth), BigInt(rect.surfaceHeight))
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
          plugin.input(events);
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
