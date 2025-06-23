import plugin from 'libentry.so';
import { resourceManager } from '@kit.LocalizationKit';

class PluginXComponentController extends XComponentController {
  resMgr: resourceManager.ResourceManager
  readyCounter = 0

  constructor(resourceManager: resourceManager.ResourceManager) {
    super()
    this.resMgr = resourceManager
  }

  trySetup() {
    if (this.readyCounter >= 2) {
      return
    }
    this.readyCounter++
    if (this.readyCounter < 2) {
      return
    }
    plugin.setup()
  }

  onSurfaceCreated(surfaceId: string): void {
    plugin.setResourceManager(this.resMgr)
    plugin.setSurfaceId(BigInt(surfaceId))
    this.trySetup()
  }

  onSurfaceChanged(surfaceId: string, rect: SurfaceRect): void {
    plugin.changeSurface(BigInt(surfaceId), BigInt(rect.surfaceWidth), BigInt(rect.surfaceHeight))
    this.trySetup()
  }

  onSurfaceDestroyed(surfaceId: string): void {
    plugin.destroySurface(BigInt(surfaceId))
    this.readyCounter = 0
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
        .width('100%')
        .height('100%')
    }
    .width('100%')
    .height('100%')
  }
}
