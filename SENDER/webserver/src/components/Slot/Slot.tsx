import { Slot } from "@/types/types"
import { Box, Group, Text } from "@mantine/core"

import classes from "./Slot.module.css"

const SlotComponent = ({ slot }: {
    slot: Slot
}) => {

    // TODO: put in context
    const url = 'http://192.168.1.1';

    const onSelectSlot = () => {
        // fetch(`${url}/slots?select=${slot.id}`)
        //     .then(response => response.json())
        //     .then(console.log)
    }

    return <Box className={classes.container} onClick={onSelectSlot}>
        <Box className={classes.lightColor} style={{
            backgroundColor: `rgb(${slot.light.r}, ${slot.light.g}, ${slot.light.b})`
        }}> </Box>

        <Text> rgb({slot.light.r}, {slot.light.g}, {slot.light.b}) </Text>
    </Box>

}

export default SlotComponent