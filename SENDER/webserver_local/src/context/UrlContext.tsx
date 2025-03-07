import { createContext } from "preact";
import { useContext, useState } from "preact/hooks";

export const UrlContext = createContext<{
    serverIp: string,
    setServerIp: (url: string) => void
}>({
    serverIp: 'http://192.168.1.1/api',
    setServerIp: () => { }
});

const UrlProvider = ({ children }: { children: any }) => {

    const [url, setUrl] = useState<string>('http://192.168.1.1/api');

    return <UrlContext.Provider value={{ serverIp: url, setServerIp: setUrl }}>
        {children}
    </UrlContext.Provider>
}

export const useUrl = () => {
    return useContext(UrlContext);
}


export default UrlProvider