import plugin from 'libentry.so';
import { resourceManager } from '@kit.LocalizationKit';
import { hilog } from '@kit.PerformanceAnalysisKit';
import { display } from '@kit.ArkUI';

class PluginXComponentController extends XComponentController {
  resMgr: resourceManager.ResourceManager

  constructor(resourceManager: resourceManager.ResourceManager) {
    super()
    this.resMgr = resourceManager
  }

  onSurfaceCreated(surfaceId: string): void {
    plugin.setResourceManager(this.resMgr)
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
  controller: XComponentController = new PluginXComponentController(this.resMgr)

  build() {
    Row() {
      XComponent({
        id: "window",
        type: XComponentType.SURFACE,
        controller: this.controller,
      })
        .onLoad(() => {
          display.getAllDisplays((err, data) => {
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
        .width('100%')
        .height('100%')
        .expandSafeArea([SafeAreaType.SYSTEM])
    }
    .width('100%')
    .height('100%')
    .expandSafeArea([SafeAreaType.SYSTEM])
  }
}
