import { resourceManager } from "@kit.LocalizationKit";

export class SimplifiedTouchEvent {
  public type: number
  public id: number
  public x: number
  public y: number
}

export const setResourceManager: (resourceManager: resourceManager.ResourceManager) => any;

export const setWindowId: (id: number) => any;

export const setSurfaceId: (id: BigInt) => any;

export const changeSurface: (id: BigInt, w: BigInt, h: BigInt) => any;

export const destroySurface: (id: BigInt) => any;

export const setup: () => any;

export const input: (events: SimplifiedTouchEvent[]) => any;