

import * as Icon from "../components/Icon.bs.js";
import * as Meta from "../components/Meta.bs.js";
import * as Util from "../common/Util.bs.js";
import * as React from "react";
import * as BlogApi from "../common/BlogApi.bs.js";
import * as DateStr from "../common/DateStr.bs.js";
import * as $$Promise from "reason-promise/src/js/promise.js";
import * as Process from "process";
import * as Markdown from "../components/Markdown.bs.js";
import * as Belt_List from "bs-platform/lib/es6/belt_List.js";
import * as Link from "next/link";
import * as Belt_Array from "bs-platform/lib/es6/belt_Array.js";
import * as MainLayout from "../layouts/MainLayout.bs.js";
import * as Belt_Option from "bs-platform/lib/es6/belt_Option.js";
import * as Caml_option from "bs-platform/lib/es6/caml_option.js";
import * as BlogFrontmatter from "../common/BlogFrontmatter.bs.js";

var Params = { };

var frontmatter = (function(component) {
        if(typeof component.frontmatter === "object") { return component.frontmatter; }
        return {};
      });

var BlogComponent = {
  frontmatter: frontmatter
};

function Blog_slug$Line(Props) {
  return React.createElement("div", {
              className: "block border-t border-snow-darker"
            });
}

var Line = {
  make: Blog_slug$Line
};

function Blog_slug$BlogHeader(Props) {
  var date = Props.date;
  var author = Props.author;
  var title = Props.title;
  var category = Props.category;
  var description = Props.description;
  var articleImg = Props.articleImg;
  var date$1 = DateStr.toDate(date);
  var displayName = BlogFrontmatter.Author.getDisplayName(author);
  var match = author.imgUrl;
  var authorImg;
  if (match !== null) {
    authorImg = React.createElement("img", {
          className: "h-full w-full rounded-full",
          src: match
        });
  } else {
    var match$1 = Belt_List.fromArray(displayName.split(" "));
    var initials;
    if (match$1) {
      var match$2 = match$1[1];
      var first = match$1[0];
      if (match$2) {
        var match$3 = match$2[1];
        var exit = 0;
        if (match$3 && !match$3[1]) {
          initials = first[0] + match$3[0][0];
        } else {
          exit = 1;
        }
        if (exit === 1) {
          initials = first[0] + match$2[0][0];
        }
        
      } else {
        initials = first[0];
      }
    } else {
      initials = "";
    }
    authorImg = React.createElement("div", {
          className: "block uppercase h-full w-full flex items-center justify-center rounded-full"
        }, React.createElement("span", {
              className: "text-xl text-night"
            }, Util.ReactStuff.s(initials)));
  }
  var match$4 = author.twitter;
  return React.createElement("div", {
              className: "flex flex-col items-center"
            }, React.createElement("div", {
                  className: "max-w-705"
                }, React.createElement("div", {
                      className: "text-night-light text-lg mb-6"
                    }, Util.ReactStuff.s(category), Util.ReactStuff.s(" · "), Util.ReactStuff.s(Util.$$Date.toDayMonthYear(date$1))), React.createElement("h1", {
                      className: "text-48 text-night-dark"
                    }, Util.ReactStuff.s(title)), Belt_Option.mapWithDefault(description, null, (function (desc) {
                        return React.createElement("div", {
                                    className: "my-8 text-night-darker"
                                  }, React.createElement(Markdown.Intro.make, {
                                        children: Util.ReactStuff.s(desc)
                                      }));
                      })), React.createElement("div", {
                      className: "flex mb-12 items-center"
                    }, React.createElement("div", {
                          className: "w-12 h-12 bg-berry-40 block rounded-full mr-3"
                        }, authorImg), React.createElement("div", {
                          className: "text-18 text-night-dark"
                        }, match$4 !== null ? React.createElement("a", {
                                className: "hover:text-night",
                                href: "https://twitter.com/" + match$4,
                                rel: "noopener noreferrer",
                                target: "_blank"
                              }, Util.ReactStuff.s(displayName)) : Util.ReactStuff.s(author.username), React.createElement("div", {
                              className: "text-night-light"
                            }, Util.ReactStuff.s(author.role))))), articleImg !== undefined ? React.createElement("div", {
                    className: "-mx-8 sm:mx-0 sm:w-full bg-night-10 md:mt-24"
                  }, React.createElement("img", {
                        className: "h-full w-full object-cover",
                        style: {
                          maxHeight: "33.625rem"
                        },
                        src: articleImg
                      })) : React.createElement("div", {
                    className: "max-w-705 w-full"
                  }, React.createElement(Blog_slug$Line, { })));
}

var BlogHeader = {
  make: Blog_slug$BlogHeader
};

var cwd = Process.cwd();

function $$default(props) {
  var module_ = require("../_blogposts/" + (props.slug + ".mdx"));
  var component = module_.default;
  var authors = BlogFrontmatter.Author.getAllAuthors(/* () */0);
  var arg = frontmatter(component);
  var fm = (function (param) {
        return BlogFrontmatter.decode(param, arg);
      })(authors);
  var children = React.createElement(component, { });
  var content;
  if (fm.tag) {
    content = React.createElement("div", undefined, React.createElement(Markdown.Warn.make, {
              children: null
            }, React.createElement("h2", {
                  className: "font-bold text-night-dark text-2xl mb-2"
                }, Util.ReactStuff.s("Could not parse file '_blogposts/" + (props.slug + ".mdx'"))), React.createElement("p", undefined, Util.ReactStuff.s("The content of this blog post will be displayed as soon as all\n            required frontmatter data has been added.")), React.createElement("p", {
                  className: "font-bold mt-4"
                }, Util.ReactStuff.s("Errors:")), Util.ReactStuff.s(fm[0])));
  } else {
    var match = fm[0];
    var tmp = { };
    var tmp$1 = Caml_option.null_to_opt(match.canonical);
    if (tmp$1 !== undefined) {
      tmp.canonical = Caml_option.valFromOption(tmp$1);
    }
    content = React.createElement("div", {
          className: "w-full"
        }, React.createElement(Meta.make, tmp), React.createElement("div", {
              className: "md:mb-32"
            }, React.createElement(Blog_slug$BlogHeader, {
                  date: match.date,
                  author: match.author,
                  title: match.title,
                  category: BlogFrontmatter.Category.toString(match.category),
                  description: Caml_option.null_to_opt(match.description),
                  articleImg: Caml_option.null_to_opt(match.articleImg)
                })), React.createElement("div", {
              className: "flex justify-center"
            }, React.createElement("div", {
                  className: "max-w-705 w-full"
                }, children, React.createElement("div", {
                      className: "mt-12"
                    }, React.createElement(Blog_slug$Line, { }), React.createElement("div", {
                          className: "pt-20 flex flex-col items-center"
                        }, React.createElement("div", {
                              className: "text-3xl sm:text-4xl text-center text-night-dark font-medium"
                            }, Util.ReactStuff.s("Want to read more?")), React.createElement(Link.default, {
                              href: "/blog",
                              children: React.createElement("a", {
                                    className: "text-fire hover:text-fire-80"
                                  }, Util.ReactStuff.s("Back to Overview"), React.createElement(Icon.ArrowRight.make, {
                                        className: "ml-2 inline-block"
                                      }))
                            }))))));
  }
  return React.createElement(MainLayout.make, {
              children: content
            });
}

function getStaticProps(ctx) {
  var props = {
    slug: ctx.params.slug
  };
  return $$Promise.resolved({
              props: props
            });
}

function getStaticPaths(param) {
  var paths = Belt_Array.map(BlogApi.getAllPosts(/* () */0), (function (postData) {
          return {
                  params: {
                    slug: postData.slug
                  }
                };
        }));
  return $$Promise.resolved({
              paths: paths,
              fallback: false
            });
}

export {
  Params ,
  BlogComponent ,
  Line ,
  BlogHeader ,
  cwd ,
  $$default ,
  $$default as default,
  getStaticProps ,
  getStaticPaths ,
  
}
/* cwd Not a pure module */
