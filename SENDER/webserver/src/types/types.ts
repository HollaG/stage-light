export interface Light {
    light__type: number,
    r: number,
    g: number,
    b: number,
    w: number,
    ww: number,
    light__transition: number,
}
export interface Slot {
    light: Light
    id: number, // just the index for now
}
export interface CGroup {
    id: number;
    name: string;
    slots: Slot[];
}

export interface Response<T> {
    data: T
}