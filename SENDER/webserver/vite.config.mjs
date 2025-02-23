import react from '@vitejs/plugin-react';
import { defineConfig } from 'vite';
import { VitePWA } from 'vite-plugin-pwa';
import tsconfigPaths from 'vite-tsconfig-paths';

export default defineConfig(({ mode }) => ({
  base: mode === 'development' ? '/' : '/stage-light/',
  plugins: [
    react(),
    tsconfigPaths(),
    VitePWA({
      registerType: 'autoUpdate',
      devOptions: {
        enabled: true,
        type: 'module',
      },
      includeAssets: ['favicon.ico', 'apple-touch-icon.png', 'mask-icon.svg'],
      manifest: {
        name: 'My Awesome App',
        short_name: 'MyApp',
        description: 'My Awesome App description',
        theme_color: '#ffffff',
        icons: [
          {
            src: mode === 'development' ? '/pwa-64x64.png' : '/stage-light/pwa-64x64.png',
            sizes: '64x64',
            type: 'image/png',
          },
          {
            src: mode === 'development' ? '/pwa-192x192.png' : '/stage-light/pwa-192x192.png',
            sizes: '192x192',
            type: 'image/png',
          },
          {
            src: mode === 'development' ? '/pwa-512x512.png' : '/stage-light/pwa-512x512.png',
            sizes: '512x512',
            type: 'image/png',
          },
          {
            src: mode === 'development' ? '/pwa-512x512.png' : '/stage-light/pwa-512x512.png',
            sizes: '512x512',
            type: 'image/png',
            purpose: 'maskable',
          },
        ],
      },
    }),
  ],
  test: {
    globals: true,
    environment: 'jsdom',
    setupFiles: './vitest.setup.mjs',
  },
}));
