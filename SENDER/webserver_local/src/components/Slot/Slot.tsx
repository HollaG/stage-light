import { Slot } from "../../types/types"

import classes from "./Slot.module.css"

const SlotComponent = ({ slot }: {
    slot: Slot
}) => {

    // TODO: put in context
    const url = 'http://192.168.1.1/api';

    const onSelectSlot = () => {
        fetch(`${url}/slots?select=${slot.id}`)
            .then(response => response.json())
            .then(console.log)
    }

    return <div className={classes.container} onClick={onSelectSlot}>
        <div className={classes.lightColor} style={{
            backgroundColor: `rgb(${slot.light.r}, ${slot.light.g}, ${slot.light.b})`
        }}> </div>

        <p> rgb({slot.light.r}, {slot.light.g}, {slot.light.b}) </p>
    </div>

}

export default SlotComponent