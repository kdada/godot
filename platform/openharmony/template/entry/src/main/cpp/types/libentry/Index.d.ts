import { resourceManager } from "@kit.LocalizationKit";

export const setResourceManager: (resourceManager: resourceManager.ResourceManager) => any;
export const setSurfaceId: (id: BigInt) => any;
export const changeSurface: (id: BigInt, w: BigInt, h: BigInt) => any;
export const destroySurface: (id: BigInt) => any;
export const setup: () => any;
