import { TargetedEvent } from "preact/compat";
import { useState } from "preact/hooks";

const ColorPicker = ({ initialColor, onChange }: {
  initialColor?: string;
  onChange?: (color: string) => void;
}) => {

  const [color, setColor] = useState(initialColor || "#000000");
  const onColorUpdate = (e: TargetedEvent<HTMLInputElement, Event>) => {
    setColor(e.currentTarget.value);

    if (onChange) {
      onChange(e.currentTarget.value)
    }
  }

  return <input type="color" id="head" name="head" value={color} onChange={onColorUpdate} />
}

export default ColorPicker