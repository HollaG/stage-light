import { Button, Container, Group, Select, Stack, TextInput } from '@mantine/core';
import { ColorSchemeToggle } from '../components/ColorSchemeToggle/ColorSchemeToggle';
import { Welcome } from '../components/Welcome/Welcome';
import { useState } from 'react';
import { useFetch } from '@mantine/hooks';
import { CGroup, Response, Slot } from '@/types/types';
import SlotComponent from '@/components/Slot/Slot';



export function HomePage() {
    const [serverIp, setServerIp] = useState<string>('http://192.168.1.131');
    // const { data, loading, error, refetch, abort } = useFetch<Response<Partial<Group>[]>>(
    //     `${serverIp}/groups`
    // );

    const [groupData, setGroupData] = useState<Response<Partial<CGroup>[]>>({ data: [] });
    const [controllerState, setControllerState] = useState<{
        groupIndex: number,
        slotIndex: number
    }>({
        groupIndex: -1,
        slotIndex: -1
    })

    const [activeGroup, setActiveGroup] = useState<CGroup>();


    const groupsValue: {
        value: string;
        label: string;
    }[] = groupData?.data.map((group) => ({
        value: group.id?.toString() || "",
        label: group.name || 'Unknown',
    })) || [];



    const onIpUpdate = (evt: React.ChangeEvent<HTMLInputElement>) => {
        setServerIp(evt.currentTarget.value);
        wrapper()
    }

    const wrapper = async () => {
        getGroups().then(() => getState());
    }

    const getGroups = async () => {
        return fetch(`${serverIp}/groups`)
            .then((response) => response.json())
            .then((data) => {
                // console.log(data);
                setGroupData(data);
            })

    }

    const getState = async () => {
        const res1 = await fetch(`${serverIp}`);
        const data1 = await res1.json();
        // console.log({ state: data1 });
        setControllerState({
            groupIndex: data1.groupIndex,
            slotIndex: data1.slotIndex
        })

        const res2 = await fetch(`${serverIp}/groups/${data1.groupIndex}`);
        const data2 = await res2.json();

        // console.log({ group: data2 });

        setActiveGroup(data2.data);


    }

    const onChangeGroup = (groupId: string) => { // groupId is just the index
        fetch(`${serverIp}/groups?select=${groupId}`)
            .then(res => res.json())
            .then((data) => {
                // TODO: handle error
                getState(); 
            })
    }

    console.log({activeGroup})

    // console.log({ groupsValue });
    return (
        <Container size="sm" style={{ padding: "4rem" }}>
            <Stack>
                <Group>

                    <TextInput
                        label="Server IP"
                        description="ESP Host IP"
                        placeholder="192.168.x.x"
                        value={serverIp}
                        onChange={onIpUpdate}
                    />
                    <Button onClick={() => wrapper()} color="blue">
                        Refresh
                    </Button>
                </Group>
                <Group>

                    <Select
                        label="Active group"
                        value={controllerState.groupIndex !== undefined ? controllerState.groupIndex.toString() : ""}
                        data={groupsValue}
                        onChange={(groupId) => {
                            // setControllerState({
                            //     groupIndex: parseInt(value),
                            //     slotIndex: controllerState.slotIndex
                            // })
                            onChangeGroup(groupId!)
                        }}
                    />
                </Group>

                <Stack>
                    {activeGroup ? activeGroup.slots.map((slot, index) => <SlotComponent slot={slot} key={index} />) : "No slots saved yet!"}
                </Stack>
            </Stack>
        </Container>
    );
}
