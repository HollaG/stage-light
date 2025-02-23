import { defineConfig } from 'vite';
import react from '@vitejs/plugin-react';
import tsconfigPaths from 'vite-tsconfig-paths';
import { VitePWA } from 'vite-plugin-pwa'

export default defineConfig({
    base: "/stage-light/SENDER/webserver",
  plugins: [react(), tsconfigPaths(), VitePWA({
    registerType: 'autoUpdate', devOptions: {
      enabled: true,
      type: "module"
    },
    includeAssets: ['favicon.ico', 'apple-touch-icon.png', 'mask-icon.svg'],
    manifest: {
      name: 'My Awesome App',
      short_name: 'MyApp',
      description: 'My Awesome App description',
      theme_color: '#ffffff',
      "icons": [
        {
          "src": "src/public/pwa-64x64.png",
          "sizes": "64x64",
          "type": "image/png"
        },
        {
          "src": "src/public/pwa-192x192.png",
          "sizes": "192x192",
          "type": "image/png"
        },
        {
          "src": "src/public/pwa-512x512.png",
          "sizes": "512x512",
          "type": "image/png"
        },
        {
          "src": "src/public/maskable-icon-512x512.png",
          "sizes": "512x512",
          "type": "image/png",
          "purpose": "maskable"
        }
      ]
    }
  })],
  test: {
    globals: true,
    environment: 'jsdom',
    setupFiles: './vitest.setup.mjs',
  },
});
