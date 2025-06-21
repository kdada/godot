import { hilog } from '@kit.PerformanceAnalysisKit';

const DOMAIN = 0x0000;

@Entry
@Component
struct Index {
  build() {
    Row() {
      XComponent({
        id: "godotsurface",
        type: XComponentType.SURFACE,
        libraryname: "entry",
      })
        .width('100%')
        .height('100%')
    }
    .width('100%')
    .height('100%')
  }
}
