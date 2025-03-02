// import preactLogo from '../../assets/preact.svg';
// import './style.css';

// export function Home() {
// 	return (
// 		<div class="home">
// 			<a href="https://preactjs.com" target="_blank">
// 				<img src={preactLogo} alt="Preact logo" height="160" width="160" />
// 			</a>
// 			<h1>Get Started building Vite-powered Preact Apps </h1>
// 			<section>
// 				<Resource
// 					title="Learn Preact"
// 					description="If you're new to Preact, try the interactive tutorial to learn important concepts"
// 					href="https://preactjs.com/tutorial"
// 				/>
// 				<Resource
// 					title="Differences to React"
// 					description="If you're coming from React, you may want to check out our docs to see where Preact differs"
// 					href="https://preactjs.com/guide/v10/differences-to-react"
// 				/>
// 				<Resource
// 					title="Learn Vite"
// 					description="To learn more about Vite and how you can customize it to fit your needs, take a look at their excellent documentation"
// 					href="https://vitejs.dev"
// 				/>
// 			</section>
// 		</div>
// 	);
// }

// function Resource(props) {
// 	return (
// 		<a href={props.href} target="_blank" class="resource">
// 			<h2>{props.title}</h2>
// 			<p>{props.description}</p>
// 		</a>
// 	);
// }

import { TargetedEvent, useEffect, useState } from 'react';
import { CGroup, Response, Slot } from '../../types/types';
import SlotComponent from '../../components/Slot/Slot';
import { useUrl } from '../../context/UrlContext';



export function HomePage() {
    // const [serverIp, setServerIp] = useState<string>('http://localhost:3000/api');
    const { serverIp, setServerIp } = useUrl()
    const [groupData, setGroupData] = useState<Response<Partial<CGroup>[]>>({ data: [] });
    const [webState, setWebState] = useState<{
        groupIndex: number,
        slotIndex: number
    }>({
        groupIndex: -1,
        slotIndex: -1
    })

    const [activeGroup, setActiveGroup] = useState<CGroup>();

    // on launch, fetch the groups, THEN the state
    const getGroups = async () => {
        return fetch(`${serverIp}/groups`)
            .then((response) => response.json())
            .then((data) => {
                // console.log(data);
                setGroupData(data);
            })

    }

    /**
     * Get the state of the controller.
     * @returns 
     */
    const getControllerState = async () => {
        const res1 = await fetch(`${serverIp}`);
        const data1 = await res1.json() as { 
            groupIndex: number, 
            slotIndex: number
        };
        

        return data1;
    }

    const getState = async () => {
        const res1 = await fetch(`${serverIp}`);
        const data1 = await res1.json();
        setWebState({
            groupIndex: data1.groupIndex,
            slotIndex: data1.slotIndex
        })

        return data1;
    }


    const getData = () => {
        getGroups().then(() => getState());
    }

    useEffect(() => {
        getGroups().then(() => changeGroup(0)) // always set to default
    }, [])

    console.log({ controllerState: webState })

    // every time the group changes, we need to fetch the DATA only of the new group
    // useEffect(() => {
    //     fetch(`${serverIp}/groups/${webState.groupIndex}`)
    //         .then(res => res.json())
    //         .then((data) => {
    //             console.log({ group: data });
    //             // setActiveGroup(data.data);
    //         })
    // }, [webState.groupIndex])

    const onGroupChange = (e: TargetedEvent<HTMLSelectElement, Event>) => {
        // setControllerState({
        //     ...controllerState,
        //     groupIndex: index
        // })
        const index = (e.currentTarget.value);
        console.log({ index })

        changeGroup(index);

    }

    const changeGroup = (index: any) => {
        fetch(`${serverIp}/groups/${index}`)
            .then(res => res.json())
            .then((data) => {
                console.log({ group: data });
                setActiveGroup(data.data);

                setWebState({
                    ...webState,
                    groupIndex: parseInt(index)
                })
            })
    }

    const sendGroupChange = () => {
        // send the group change to the controller
        fetch(`${serverIp}/groups?select=${webState.groupIndex}`)
            .then(res => res.json())
            .then((data) => {
                // console.log({ group: data });
                // setActiveGroup(data.data);
                // TODO: error handling
            })
    }

    const syncHandler = () => {
        getControllerState().then((data) => changeGroup(data.groupIndex))
    }




    const groupsValue: {
        value: string;
        label: string;
    }[] = groupData?.data.map((group) => ({
        value: group.id?.toString() || "",
        label: group.name || 'Unknown',
    })) || [];



    return (
        <div style={{ padding: "4rem" }}>
            <div>
                <div>

                    <input
                        // label="Server IP"
                        // description="ESP Host IP"
                        placeholder="192.168.x.x"
                        value={serverIp}
                    // onChange={onIpUpdate}
                    />
                    <button onClick={() => {
                        // wrapper() 
                    }} >
                        Refresh me
                    </button>
                </div>
                <div>

                    <select
                        // label="Active group"
                        value={webState.groupIndex}
                        // data={groupsValue}
                        onChange={onGroupChange}
                    >
                        {groupsValue.map((group, index) => <option value={group.value} key={index}>{group.label}</option>)}

                    </select>
                    <button onClick={sendGroupChange}>
                        Send to controller
                    </button>
                    <button onClick={syncHandler}> Sync </button>
                </div>

                <div>
                    {activeGroup ? activeGroup.slots.map((slot, index) => <SlotComponent slot={slot} key={index} />) : "No slots saved yet!"}
                </div>
            </div>
        </div>
    );
}
