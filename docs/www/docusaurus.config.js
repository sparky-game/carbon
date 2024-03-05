import { themes as prismThemes } from 'prism-react-renderer';

const config = {
  title: 'Carbon',
  tagline: 'A simple, lightweight, straightforward C/C++ unit testing framework written in pure C.',
  favicon: 'img/favicon.svg',
  url: 'https://iwas-coder.github.io',
  baseUrl: '/carbon',
  organizationName: 'iWas-Coder',
  projectName: 'carbon',
  trailingSlash: false,
  onBrokenLinks: 'throw',
  onBrokenMarkdownLinks: 'warn',

  i18n: {
    defaultLocale: 'en',
    locales: ['en']
  },

  presets: [
    [
      'classic',
      {
        docs: {
          sidebarPath: './sidebars.js',
          editUrl: 'https://github.com/iWas-Coder/carbon/blob/master/docs/www/docs/',
          showLastUpdateTime: true
        },
        blog: {
          showReadingTime: true,
          editUrl: 'https://github.com/iWas-Coder/carbon/blob/master/docs/www/blog/'
        },
        theme: {
          customCss: './src/css/custom.css'
        }
      }
    ]
  ],

  themeConfig: ({
    image: 'img/docusaurus-social-card.jpg',
    navbar: {
      logo: {
        alt: 'Carbon Logo',
        src: 'img/logo.png'
      },
      items: [
        {
          href: 'https://github.com/iWas-Coder/carbon',
          className: 'header-github-link',
          'aria-label': 'GitHub',
          position: 'right'
        },
        {
          type: 'docSidebar',
          sidebarId: 'docs',
          position: 'left',
          label: 'Docs'
        },
        {
          to: 'blog',
          position: 'left',
          label: 'Blog'
        }
      ],
    },
    algolia: {
      appId: 'NQFRICJH33',
      apiKey: 'b7e7597d510e0f0a58977359d95f64ca',
      indexName: 'iwas-coderio',
      contextualSearch: true
    },
    footer: {
      style: 'dark',
      links: [
        {
          title: 'Docs',
          items: [
            {
              label: 'What is Carbon?',
              to: 'docs/overview/1.1-what-is-carbon'
            },
            {
              label: 'Getting started',
              to: 'docs/getting-started/2.1-getting-started'
            }
          ]
        },
        {
          title: 'Community',
          items: [
            {
              label: 'Contributing',
              href: 'https://github.com/iWas-Coder/carbon/blob/master/CONTRIBUTING.org'
            }
          ]
        },
        {
          title: 'More',
          items: [
            {
              label: 'GitHub',
              href: 'https://github.com/iWas-Coder/carbon',
            }
          ]
        }
      ],
      copyright: `Copyright © ${new Date().getFullYear()} Wasym Atieh Alonso. All rights reserved.`,
    },
    prism: {
      theme: prismThemes.github,
      darkTheme: prismThemes.dracula,
    }
  })
};

export default config;
