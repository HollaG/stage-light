import { Dispatch, StateUpdater } from "preact/hooks";
import { useUrl } from "../../context/UrlContext";
import { Slot } from "../../types/types"
import Badge from "../Badge/Badge";

import classes from "./Slot.module.css"

const SlotComponent = ({ slot, groupId, refresh, setWebState, webState }: {
    slot: Slot,
    groupId: number,
    refresh: () => void,
    webState: {
        groupIndex: number;
        slotIndex: number;
    },
    setWebState: Dispatch<StateUpdater<{
        groupIndex: number;
        slotIndex: number;
    }>>
}) => {

    const { serverIp, setServerIp } = useUrl()

    const onSelectSlot = (e: React.MouseEvent<HTMLDivElement>) => {

        setWebState(prev => ({
            slotIndex: slot.id,
            groupIndex: groupId
        }))

    }

    const onDeleteSlot = (e: React.MouseEvent<HTMLDivElement>) => {
        e.preventDefault();
        e.stopPropagation()
        fetch(`${serverIp}/slots`, {
            method: "DELETE",
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({
                groupIndex: groupId,
                slotIndex: slot.id
            })
        }).then(res => {
            if (res.ok) {
                console.log("Deleted slot")

                refresh();
            }
        })
    }

    // return <div className={classes.container} onClick={onSelectSlot}>
    //     <div className={classes.lightColor} style={{
    //         backgroundColor: `rgb(${slot.light.r}, ${slot.light.g}, ${slot.light.b})`
    //     }}> </div>

    //     <p> rgb({slot.light.r}, {slot.light.g}, {slot.light.b}) </p>
    // </div>

    return <article className={`${classes.slotContainer} ${webState.slotIndex === slot.id ? classes.outlined : undefined}`} onClick={onSelectSlot}>


        <div className={classes.lightColor} style={{
            backgroundColor: `rgb(${slot.light.r}, ${slot.light.g}, ${slot.light.b})`
        }}> </div>
        <p> rgb({slot.light.r}, {slot.light.g}, {slot.light.b}) </p>
        {/* <Badge color={"success"}>
            ACTIVE
        </Badge> */}

        <div style={{ flex: 1 }}></div>

        <details class="dropdown" style={{ margin: 0 }} onClick={(e) => e.stopPropagation()}>
            <summary>Actions</summary>
            <ul>
                <li><div onClick={onDeleteSlot}>Delete</div></li>
                <li><a href="#">Move Up</a></li>
                <li><a href="#">Move Down</a></li>
                <li><a href="#">Edit</a></li>
            </ul>
        </details>

    </article>

}

export default SlotComponent